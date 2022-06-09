
#include "include/hal/esp32-c3/esp32_c3_CaptivePortalDNS.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "lwip/err.h"
#include "tcpip_adapter.h"
#include "string.h"


#define FLAG_QR (1<<7)
#define FLAG_AA (1<<2)
#define FLAG_TC (1<<1)
#define FLAG_RD (1<<0)

#define QTYPE_A  1
#define QTYPE_NS 2
#define QTYPE_CNAME 5
#define QTYPE_SOA 6
#define QTYPE_WKS 11
#define QTYPE_PTR 12
#define QTYPE_HINFO 13
#define QTYPE_MINFO 14
#define QTYPE_MX 15
#define QTYPE_TXT 16
#define QTYPE_URI 256

#define QCLASS_IN 1
#define QCLASS_ANY 255
#define QCLASS_URI 256

#define DNS_LEN 512

int esp32_c3_CaptivePortalDNS::s_sockFd = 0;

esp32_c3_CaptivePortalDNS::esp32_c3_CaptivePortalDNS() {
    xTaskCreate(captdnsTask, (const char *)"captdns_task", 10000, NULL, 3, &m_taskHandle);
    
}


esp32_c3_CaptivePortalDNS::~esp32_c3_CaptivePortalDNS() {
    vTaskDelete(m_taskHandle);
}

static void  setn16(void *pp, int16_t n) {
	char *p=(char *)pp;
	*p++=(n>>8);
	*p++=(n&0xff);
}

static void  setn32(void *pp, int32_t n) {
	char *p=(char *)pp;
	*p++=(n>>24)&0xff;
	*p++=(n>>16)&0xff;
	*p++=(n>>8)&0xff;
	*p++=(n&0xff);
}

static uint16_t  my_ntohs(uint16_t *in) {
	char *p=(char*)in;
	return ((p[0]<<8)&0xff00)|(p[1]&0xff);
}

static char*  labelToStr(char *packet, char *labelPtr, int packetSz, char *res, int resMaxLen) {
	int i, j, k;
	char *endPtr=NULL;
	i=0;
	do {
		if ((*labelPtr&0xC0)==0) {
			j=*labelPtr++;
			if (i<resMaxLen && i!=0) res[i++]='.';
			for (k=0; k<j; k++) {
				if ((labelPtr-packet)>packetSz) return NULL;
				if (i<resMaxLen) res[i++]=*labelPtr++;
			}
		} else if ((*labelPtr&0xC0)==0xC0) {
			endPtr=labelPtr+2;
			int offset=my_ntohs(((uint16_t *)labelPtr))&0x3FFF;
			if (offset>packetSz) return NULL;
			labelPtr=&packet[offset];
		}
		if ((labelPtr-packet)>packetSz) return NULL;
	} while (*labelPtr!=0);
	res[i]=0;
	if (endPtr==NULL) endPtr=labelPtr+1;
	return endPtr;
}

static char  *strToLabel(char *str, char *label, int maxLen) {
	char *len=label; 
	char *p=label+1; 
	while (1) {
		if (*str=='.' || *str==0) {
			*len=((p-len)-1);	
			len=p;				
			p++;				
			if (*str==0) break;	
			str++;
		} else {
			*p++=*str++;	
            	}
	}
	*len=0;
	return p;
}


void esp32_c3_CaptivePortalDNS::captdnsTask(void *pvParameters) {
    struct sockaddr_in server_addr;
	uint32_t ret;
	struct sockaddr_in from;
	socklen_t fromlen;
	
	char udp_msg[DNS_LEN];
	
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;	   
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(53);
	server_addr.sin_len = sizeof(server_addr);
	
	do {
		s_sockFd=socket(AF_INET, SOCK_DGRAM, 0);
		if (s_sockFd==-1) {
			printf("captdns_task failed to create sock!\n");
			vTaskDelay(1000/portTICK_RATE_MS);
		}
	} while (s_sockFd==-1);
	
	do {
		ret=bind(s_sockFd, (struct sockaddr *)&server_addr, sizeof(server_addr));
		if (ret!=0) {
			printf("captdns_task failed to bind sock!\n");
			vTaskDelay(1000/portTICK_RATE_MS);
		}
	} while (ret!=0);

    printf("CaptDNS inited.\n");
	
	while(1) {
		memset(&from, 0, sizeof(from));
		fromlen=sizeof(struct sockaddr_in);
		ret=recvfrom(s_sockFd, (uint8_t *)udp_msg, DNS_LEN, 0,(struct sockaddr *)&from,(socklen_t *)&fromlen);
		if (ret>0) captdnsRecv(&from,udp_msg,ret);
	}
	
	close(s_sockFd);
	vTaskDelete(NULL);
}


void  esp32_c3_CaptivePortalDNS::captdnsRecv(struct sockaddr_in *premote_addr, char *pusrdata, unsigned short length) {

	char buff[DNS_LEN];
	char reply[DNS_LEN];
	int i;
	char *rend=&reply[length];
	char *p=pusrdata;
	DnsHeader *hdr=(DnsHeader*)p;
	DnsHeader *rhdr=(DnsHeader*)&reply[0];
	p+=sizeof(DnsHeader);

	if (length>DNS_LEN) return;
	if (length<sizeof(DnsHeader)) return;
	if (hdr->ancount || hdr->nscount || hdr->arcount) return;
	if (hdr->flags&FLAG_TC) return;

	memcpy(reply, pusrdata, length);
	rhdr->flags|=FLAG_QR;

	for (i=0; i<my_ntohs(&hdr->qdcount); i++) 
	{
		p=labelToStr(pusrdata, p, length, buff, sizeof(buff));
		if (p==NULL) return;
		DnsQuestionFooter *qf=(DnsQuestionFooter*)p;
		p+=sizeof(DnsQuestionFooter);
		

		if (my_ntohs(&qf->type)==QTYPE_A) {
			
			rend=strToLabel(buff, rend, sizeof(reply)-(rend-reply)); 
			if (rend==NULL) return;
			DnsResourceFooter *rf=(DnsResourceFooter *)rend;
			rend+=sizeof(DnsResourceFooter);
			setn16(&rf->type, QTYPE_A);
			setn16(&rf->dclass, QCLASS_IN);
			setn32(&rf->ttl, 0);
			setn16(&rf->rdlength, 4); //IPv4 addr is 4 bytes;
            tcpip_adapter_ip_info_t info;
            tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &info);
			*rend++=ip4_addr1(&info.ip);
			*rend++=ip4_addr2(&info.ip);
			*rend++=ip4_addr3(&info.ip);
			*rend++=ip4_addr4(&info.ip);
			setn16(&rhdr->ancount, my_ntohs(&rhdr->ancount)+1);
	    
		} else if (my_ntohs(&qf->type)==QTYPE_NS) {
			rend=strToLabel(buff, rend, sizeof(reply)-(rend-reply)); 
			DnsResourceFooter *rf=(DnsResourceFooter *)rend;
			rend+=sizeof(DnsResourceFooter);
			setn16(&rf->type, QTYPE_NS);
			setn16(&rf->dclass, QCLASS_IN);
			setn16(&rf->ttl, 0);
			setn16(&rf->rdlength, 4);
			*rend++=2;
			*rend++='n';
			*rend++='s';
			*rend++=0;
			setn16(&rhdr->ancount, my_ntohs(&rhdr->ancount)+1);
            
		} else if (my_ntohs(&qf->type)==QTYPE_URI) {
			rend=strToLabel(buff, rend, sizeof(reply)-(rend-reply));
			DnsResourceFooter *rf=(DnsResourceFooter *)rend;
			rend+=sizeof(DnsResourceFooter);
			DnsUriHdr *uh=(DnsUriHdr *)rend;
			rend+=sizeof(DnsUriHdr);
			setn16(&rf->type, QTYPE_URI);
			setn16(&rf->dclass, QCLASS_URI);
			setn16(&rf->ttl, 0);
			setn16(&rf->rdlength, 4+20);
			setn16(&uh->prio, 10);
			setn16(&uh->weight, 1);
                        memcpy(rend, "http://pocuter.nonet", 20);
			rend+=20;
			setn16(&rhdr->ancount, my_ntohs(&rhdr->ancount)+1);
            
		}
	}	
	sendto(s_sockFd,(uint8_t*)reply, rend-reply, 0, (struct sockaddr *)premote_addr, sizeof(struct sockaddr_in));

}



