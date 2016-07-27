/*
//gcc zbclient.c -o zbclient -lwiringPi -lcurl -lpthread -ljson-c -lmicrohttpd
sudo cp json-c/* /usr/lib/

#build libmicrohttp.so
donwload src
cp to raspi
sudo apt-get install autoconf
sudo apt-get install litool
autoreconf -fi
./configure --disable-curl
sudo make
sudo make install
sudo cp /usr/local/lib/libmicrohttp.* /lib/arm-linux-gnueabihf/
regcc .c
ok

=====
sudo rm /usr/arm-linux-gnueabihf/libmicrohttpd.so
sudo cp libmicrohttpd.so.12.37.0 /usr/arm-linux-gnueabihf/
sudo ln -s libmicrohttpd.so.12.37.0 libmicrohttpd.so.12
sudo ln -s libmicrohttpd.so.12 libmicrohttpd.so

----
ls -l |grep libmicrohttpd
sudo find / -name libmicrohttpd*
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <string.h>
#include "json.h"
#include <time.h>
#include <stddef.h>
#include <curl.h>
#include <fcntl.h>
#include <termios.h>


#define XMKG_M_ID     		0X2d81 //a
#define XMKG_X_ID      		0Xeb38
#define XMKG_ENTER_ID      	0Xced9
#define XMKG_DOOR_ID      	0Xf9a3
#define XIAOMIRENTI_ID		0x9072
#define XIAOMIMENCI_ID		0x0c78

#define LIGHT_INSIDE_ID     0x8726
#define LIGHT_OUTSIDE_ID    0X5292
#define SWITCH_ALAM_ID      0X1294
#define SWITCH_OUTSIDE_ID   0X5de1
#define POWER_ID       		0X444e
#define DOOR_IN_ID       	0X268c //??·??
#define DOOR_OUT_ID       	0X8F46 

#define SWITCH2_ID_1      0X1294
#define SWITCH2_ID_2      0X5de1
#define SWITCH2_ID_3      0X0000
#define SWITCH2_ID_4      0X5292
#define SWITCH2_ID_5      0X8726
#define SWITCH2_ID_6      0X0000


#define SWITCH3_ID_3      0X5de1
#define SWITCH3_ID_4      0X8726
#define SWITCH3_ID_5      0X1294
#define SWITCH3_ID_6      0X5292

#define SWITCH3_ID_7      0Xa77a
#define SWITCH3_ID_8      0X745e
#define SWITCH3_ID_9      0X4ed0
#define SWITCH3_ID_10     0X9b1c

#define SWITCH3_ID_1      0X0000
#define SWITCH3_ID_2     0X0000

#define POWER_ID_1         0X42f8
#define POWER_ID_2         0X41ed
#define POWER_ID_3         0X3dd7
#define POWER_ID_4         0X1181

#define POWER_ID_5         0X9b13
#define POWER_ID_6         0X25c3
#define POWER_ID_7         0X0c00
#define POWER_ID_8         0X6f01

#define XIAOMIKAIGUAN1_ID 	  0X7e94
#define XIAOMIKAIGUAN2_ID 	  0X1f07
#define XIAOMIKAIGUAN3_ID 	  0Xf758
#define XIAOMIKAIGUAN4_ID 	  0Xbb04
#define XIAOMIKAIGUAN5_ID 	  0Xa199
#define XIAOMIKAIGUAN6_ID 	  0X8b4f

#define XIAOMIMENCI1_ID 	  0X61dd
#define XIAOMIMENCI2_ID 	  0Xc6b6


#define SWITCHN1_ID_1 	 	  0X968f
#define SWITCHN2_ID_1 	 	  0Xf345
#define SWITCHN3_ID_1 	 	  0Xe768
#define POWERN_ID_1 	 	  0X96E9





typedef unsigned char   uint8_t;     //?— ??|?·8????°
typedef unsigned short uint16_t;  


#define WW_STATE      0x55
#define AA_STATE      0xaa
#define CMD_STATE1     0x02
#define LEN_STATE      0x03
#define DATA_STATE     0x04
#define FCS_STATE      0x05

#define PORT            8888
#define PORT_CLIENT     10080

#define POSTBUFFERSIZE  512
#define MAXNAMESIZE     3000
#define MAXANSWERSIZE   512
#define GET             0
#define POST            1

#define MXJ_GATEWAY_CLUSTERID        0x01
  
#define MXJ_PM25_CLUSTERID           0x02
#define MXJ_HUMAN_CLUSTERID          0x03
#define MXJ_SWITCH_CLUSTERID         0x04
#define MXJ_LIGHT_CLUSTERID          0x05
#define MXJ_POWER_CLUSTERID          0x06
#define MXJ_REMOTE_CLUSTERID         0x07
#define MXJ_LIGHT_SENSOR_CLUSTERID   0x08
#define MXJ_TEMPERATURE_CLUSTERID    0x09
#define MXJ_DOOR_CLUSTERID           0x0A
#define MXJ_REV1_CLUSTERID           0x0B
#define MXJ_REV2_CLUSTERID           0x0C
#define MXJ_REV3_CLUSTERID           0x0D
#define MXJ_REV4_CLUSTERID           0x0E
#define MXJ_REV5_CLUSTERID           0x0F
  
#define MXJ_CTRL_DOWN                0x00
#define MXJ_CTRL_UP                  0x01
#define MXJ_REGISTER_REQUEST         0x02
#define MXJ_REGISTER_OK              0x03
#define MXJ_REGISTER_FAILED          0x04
#define MXJ_GET_STATE                0x05
#define MXJ_SEND_STATE               0x06
#define MXJ_SENSOR_DATA              0x07
#define MXJ_CONFIG_WRITE             0x08
#define MXJ_CONFIG_RETURN            0x09
#define MXJ_CONFIG_GET               0x0A
#define MXJ_GET_IDXS                 0x0B
#define MXJ_SEND_IDXS                0x0C
#define MXJ_SEND_RESPONSE            0x0D 
#define MXJ_PING_REQUEST             0x0F
#define MXJ_DEVICE_ANNCE             0x0E
#define MXJ_PING_RESPONSE            0x10
#define MXJ_SEND_RESET               0x11  
#define MXJ_XIAOMI18                 0x18
#define MXJ_XIAOMI1C                 0x1C
#define MXJ_SEND_DATA                0xff




int permitjoin = 0;

int usart_fd;
  
uint8_t state = WW_STATE;
uint8_t  LEN_Token;
uint8_t  FSC_Token;
uint8_t  tempDataLen = 0;
uint8_t rxbuf[200];
uint8_t len_global=0;
int led_state = 0;
struct termios stNew;
struct termios stOld;


time_t now;
struct tm *tblock; 

CURL* posturl;

void send_usart(uint8_t *data,uint8_t len);
void MXJ_SendRegisterMessage( uint16_t , uint8_t );
void MXJ_SendPingMessage( uint16_t id );
void MXJ_GetIdxMessage( uint16_t id );

void MXJ_SendCtrlMessage( uint16_t ,uint8_t len, uint8_t , uint8_t , uint8_t );
void MXJ_SendStateMessage( uint16_t );

void MXJ_GetStateMessage( uint16_t id );
int SerialInit(void);


typedef struct
{
  uint8_t data[100];
  uint8_t len;
} MXJFIFO;

#define FIFO_SIZE 100
MXJFIFO fifo[FIFO_SIZE];

uint8_t fifo_size = 0;
uint8_t fifo_jinwei = 0;
uint8_t fifo_start = 0;
uint8_t fifo_end = 0;


int post_type;

struct connection_info_struct
{
  int connectiontype;
  char *answerstring;
  struct MHD_PostProcessor *postprocessor;
};

const char *askpage = "{\"code\":\"0\",\"message\":\"success\"}";

const char *greetingpage =
  "<html><body><h1>Welcome, %s!</center></h1></body></html>";

const char *errorpage =
  "<html><body>This doesn't seem to be right.</body></html>";

const char *idolpage =
  "<html><body>hello idol</body></html>";


long writer(void *data, int size, int nmemb, char *content)
{
	return 1;
}

static int
send_page (struct MHD_Connection *connection, const char *page)
{
  int ret;
  struct MHD_Response *response;


  response =
    MHD_create_response_from_buffer (strlen (page), (void *) page,
				     MHD_RESPMEM_PERSISTENT);
  if (!response)
    return MHD_NO;

  //MHD_add_response_header (response, "Content-Type", "text/html;charset=UTF-8");

  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}


static int
iterate_post (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
              const char *filename, const char *content_type,
              const char *transfer_encoding, const char *data, uint64_t off,
              size_t size)
{
  struct connection_info_struct *con_info = coninfo_cls;
  if(size == 0) return MHD_NO;

	

  return MHD_YES;
}



static void
request_completed (void *cls, struct MHD_Connection *connection,
                   void **con_cls, enum MHD_RequestTerminationCode toe)
{
  struct connection_info_struct *con_info = *con_cls;
  if (NULL == con_info)
    return;

  if (con_info->connectiontype == POST)
    {
      MHD_destroy_post_processor (con_info->postprocessor);
    }
  free (con_info);
  *con_cls = NULL;
}


static int
answer_to_connection (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
	struct connection_info_struct *con_info = *con_cls;
int i=-1;
int len2 =0;


char *ToUserName;
char *CreateTime;
char *FromUserName;

char *re_body;
  FILE *sp;
  //printf ("====New %s request for %s using version %s\n", method, url, version);
 
  time(&now);
  tblock = localtime(&now);

  if (NULL == *con_cls)
    {		
      struct connection_info_struct *con_info;

      con_info = malloc (sizeof (struct connection_info_struct));
      if (NULL == con_info)
      {
        //printf("return MHD_NO \n");
        return MHD_NO;
      }
      con_info->answerstring = NULL;
      //printf("method2 =%s \n",method);

      *con_cls = (void *) con_info;

      return MHD_YES;
    }
  


  if (0 == strcmp (method, "GET"))
    {	
		//printf("get json_str = %s\n",json_str);
			return send_page (connection, errorpage);
    }
  
  //printf("method= %s\n", method);

  if (0 == strcmp (method, "POST"))
  	{  		
	  int ret;

	  if(0 == strcmp (url, "/zbClient/API/permit"))
		{
			permitjoin = 1;
			printf("POST RECIEVE:time: %d-%d-%d %d:%d:%d len=0 url=%s version=%s body=NULL\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,len2, url, version);
		 	if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		    {
			    fprintf(sp,"POST RECIEVE:time=%d-%d-%d %d:%d:%d len=0 url=%s version=%s body=NULL\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec, url, version);
			    fclose(sp);
		    }
		}


	  if (*upload_data_size != 0)
	  {

		
		
		const char* length = MHD_lookup_connection_value (connection, MHD_HEADER_KIND, MHD_HTTP_HEADER_CONTENT_LENGTH); 	
			const char* body = MHD_lookup_connection_value (connection, MHD_POSTDATA_KIND, NULL);		
			//printf("length=%s\n",length);
			//printf("body=%s\n",body);
			//printf("url=%s\n",url); 
			if(length != NULL && body != NULL)
			{		  
				len2 = atoi(length);
				//printf("len2=%d\n",len2); 
				//re_body = (uint8_t*)calloc(len2,sizeof(uint8_t));
				//strncpy(re_body,body,len2);
				printf("POST RECIEVE:time=%d-%d-%d %d:%d:%d len=%d url=%s version=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,len2, url, version,body);
			}
		
			if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
			{
				fprintf(sp,"POST RECIEVE:time=%d-%d-%d %d:%d:%d len=%d url=%s version=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,len2, url, version,body);
				fclose(sp);
			}
		
			if(len2 != 0)
			{
				json_object *my_object = json_tokener_parse(body);
				if(json_object_to_json_string(my_object) != NULL && (0 != strcmp (json_object_to_json_string(my_object), "null")))
				{
					  uint16_t address=0;
					  uint8_t index=0,resourceSum=0,commandint=255;
					  const char * command;
					  post_type = 1;
					  
					  json_object *Jaddress = NULL;
					  json_object_object_get_ex(my_object, "address",&Jaddress);
					  json_object *Jindex = NULL;
					  json_object_object_get_ex(my_object, "index",&Jindex);
					  json_object *Jcommand = NULL;
					  json_object_object_get_ex(my_object, "command",&Jcommand);
					  json_object *JresourceSum = NULL;
					  json_object_object_get_ex(my_object, "resourceSum",&JresourceSum);
					  
					  address = (uint16_t)json_object_get_int(Jaddress);
					  index = (uint16_t)json_object_get_int(Jindex);
					  command = json_object_to_json_string(Jcommand);
					  resourceSum = (uint16_t)json_object_get_int(JresourceSum);
					  //printf("address=%d\n", address);
					  //printf("index=%d\n", index);
					  //printf("command=%s\n", command);
					  //printf("resourceSum=%d\n", resourceSum);
					  //printf("recieve post json= %s\n", json_object_to_json_string(my_object));

					    if(0 == strcmp (url, "/zbClient/API/feedback/register"))
					  	{
					  		//printf("register ok\n");
							MXJ_SendRegisterMessage( address, MXJ_REGISTER_OK);
					  	}
						if(0 == strcmp (url, "/zbClient/API/delete"))
					  	{
					  		//printf("delete\n");
							MXJ_SendRegisterMessage( address, MXJ_REGISTER_FAILED);
					  	}
						if(0 == strcmp (url, "/zbClient/API/get/state"))
					  	{
					  		//printf("get state\n");
							MXJ_GetStateMessage(address);
					  	}
						if(0 == strcmp (url, "/zbClient/API/send"))
					  	{
					  		//printf("control\n");
					  		if(0 == strcmp (command, "\"On\""))
					  		{
					  			commandint = 1;
					  		}
					  		if(0 == strcmp (command, "\"Off\""))
					  		{
					  			commandint = 0;
					  		}
					  		if(0 == strcmp (command, "\"Hold\""))
					  		{
					  			commandint = 2;
					  		}							
					  		if(0 == strcmp (command, "\"Reverse\""))
					  		{
					  			commandint = 3;
					  		}
							//printf("commandint=%d\n",commandint);

							if(commandint!=255)
							{
								if(index == 1)
								{
									MXJ_SendCtrlMessage(address ,resourceSum,commandint , 2 , 2 );
								}
								else if(index == 2)
								{
									MXJ_SendCtrlMessage(address ,resourceSum, 2 ,commandint, 2 );
								}
								else if(index == 3)
								{
									MXJ_SendCtrlMessage(address ,resourceSum, 2, 2 , commandint );
								}
							}
					  	}
						if(0 == strcmp (url, "/zbClient/API/info/get"))
					  	{
					  		//printf("get device\n");
							MXJ_GetIdxMessage(address);
					  	}
						if(0 == strcmp (url, "/zbClient/API/ping"))
					  	{
					  		//printf("PING request\n");
							MXJ_SendPingMessage(address);
					  	}
						if(0 == strcmp (url, "/zbClient/API/value/get"))
					  	{
					  		//printf("get data\n");
							MXJ_GetStateMessage(address);
					  	}
						
						

						
				}
				
			}
		
		*upload_data_size = 0;
		return MHD_YES;
	  }
	  else if(post_type == 2)
	  	  return send_page (connection,askpage);
	  else if(post_type == 1)
		  return send_page (connection,askpage);
	  else
		  return send_page (connection, askpage);  
  	}


  return send_page (connection, errorpage);
}



uint8_t fifo_add(uint8_t *data,uint8_t len)
{
	uint8_t ii;
	fifo[fifo_end].len=len;
  for(ii=0;ii<len;ii++)
	{
		fifo[fifo_end].data[ii]=data[ii];
	}
  
  fifo_end++;
  if(fifo_end >= FIFO_SIZE)
  {
      fifo_end=0;
      fifo_jinwei=1;
  }
  
  return 1;
}

uint8_t fifo_read(uint8_t **data1,uint8_t *len)
{
  if(fifo_jinwei == 0)
  {
    if(fifo_start >= fifo_end)
    {
      return 0;
    }
  }
  
  *data1 = fifo[fifo_start].data;
  *len = fifo[fifo_start].len;
  fifo_start++;
  if(fifo_start >= FIFO_SIZE)
  {
    fifo_start = 0;
    fifo_jinwei = 0;
  }
  return 1;
}



void send_usart(uint8_t *data,uint8_t len) //id,state1,state2,state3 1=?a,0=1?,2=±￡3?
{
  uint8_t txbuf[100];
  uint8_t i=0;
  uint8_t crc = 0;
  txbuf[0] = 0x55;
  txbuf[1] = 0xaa;
  digitalWrite(27,HIGH);
  led_state = 0;
  for(i=0;i<len;i++)
  {
    txbuf[i+2] = data[i];
    crc += data[i];
  }
  txbuf[len+2] = crc;
  //printf("\n");
  //printf("fifo_add %d:\n",len + 3);

  fifo_add(txbuf,len+3);
  	/*
  for(i=0;i<len+3;i++)
  {
    serialPutchar(usart_fd,txbuf[i]);
    printf("%02x ",txbuf[i]);
  }
  printf("\n");
  */
}

void thread_send(void)
{
	while(1)
	{
		uint8_t i;
		uint8_t *txbuf;
		uint8_t len=0;	

		if(digitalRead (25) == 1)
			digitalWrite(24,HIGH);
		else
			digitalWrite(24,LOW);

		FILE *sp;
		if(fifo_read(&txbuf,&len))
		{			
			time(&now);
			tblock = localtime(&now);
			
			if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
			{
				fprintf(sp,"USART SEND:time=%d-%d-%d %d:%d:%d len=%d data=", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,len);
				for(i=0;i<len;i++)
				  {
				    fprintf(sp,"%02x ",txbuf[i]);
				  }
				  fprintf(sp,"\n");
				  fclose(sp);
			}
			
			printf("USART SEND:time=%d-%d-%d %d:%d:%d len=%d data=", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,len);

			write(usart_fd, txbuf, len);

			
			for(i=0;i<len;i++)
			{
				//serialPutchar(usart_fd,txbuf[i]);
				
				printf("%02x ",txbuf[i]);
			}
			printf("\n");
		}

		if(permitjoin == 1)
		{
			digitalWrite(22,LOW);
			permitjoin++;
		}
		if(permitjoin >1)
		{			
			permitjoin++;
		}		  

		usleep(5000);
		if(permitjoin > 40)
		{
			permitjoin = 0;
			digitalWrite(22,HIGH);
		}

		
		led_state++;
		if(led_state >= 10)
		{
			digitalWrite(27,LOW);
			led_state = 0;
		}
		
		//sleep(1);
	}
}



//========thread start=========//
//AA C0 IDH IDL STATE1 STATE2 STATE3 REV CRC AB


void recieve_usart(uint8_t *rx,uint8_t len)
{
  int i=0,j=0;
  int id=0,cid=0;
  digitalWrite(27,HIGH);
  led_state = 0;
  time(&now);
  tblock = localtime(&now);
  //printf("\n");
  //printf("recieved:%d - at: %d-%d-%d %d:%d:%d\n",len,tblock->tm_year, tblock->tm_mon, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);
  printf("USART RECIEVE:time=%d-%d-%d %d:%d:%d len=%d data=", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,len);
  for(i=0;i<len;i++)
	  printf("%02x ",rx[i]);
  printf("\n");


  
  FILE *sp;
  if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
	{
		fprintf(sp,"USART RECIEVE:time=%d-%d-%d %d:%d:%d len=%d data=", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,len);
		for(i=0;i<len;i++)
    		fprintf(sp,"%02x ",rx[i]);
		fprintf(sp,"\n");
		fclose(sp);
	}
  

  
 
  if(len>=4)
	{
		id=rx[2];
		id <<= 8;
		id |= rx[3];
	}

  if(len>=6)
	{
		cid=rx[4];
		cid <<= 8;
		cid |= rx[5];
	}
  
  switch(rx[0])
  {
    case MXJ_CTRL_UP:
	{
		char* event;
		switch(rx[9])
		{
			case 0: event = "PressDown";break;
			case 1: event = "PressUp";break;
			case 2: event = "DoubleClick";break;
			case 3: event = "Click";break;
			case 4: event = "BodyMove";break;
			case 5: event = "ReportData";break;
			case 6: event = "LongPress";break;
			case 7: event = "TriggerByCloud";break;	
			default : event = "Unknow event";
		}
		char str[200]={0};
		char str_url[200]={0};
		sprintf(str,"{\"address\":\"%d\",\"indaddressex\":\"%d\",\"event\":\"%s\",\"linkQuality\":\"%d\"}",id,rx[8],event,rx[len-1]);
		sprintf(str_url,"10.28.1.28:%d/device/API/command",PORT_CLIENT);
		curl_easy_setopt(posturl, CURLOPT_URL, str_url);
		curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
		curl_easy_perform(posturl);	
		
		printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
		if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		{
			fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
			  fclose(sp);
		}
						
	break;
	}


    
    case MXJ_REGISTER_REQUEST:
      	//MXJ_SendRegisterMessage(id,MXJ_REGISTER_OK);
    {
      	char* deviceType;
      	switch(cid)
  		{
			case 1: deviceType = "Gateway";break;
			case 2: deviceType = "PM2.5Sensor";break;
			case 3: deviceType = "BodySensor";break;
			case 4: deviceType = "N_SwitchLightPanel";break;
			case 5: deviceType = "FIVE";break;
			case 6: deviceType = "PowerPanel";break;
			case 7: deviceType = "N_Button";break;
			case 8: deviceType = "LightSensor";break;
			case 9: deviceType = "TemperatureSensor";break;
			case 10: deviceType = "MagnetSensor";break;
			case 11: deviceType = "SmokeSensor";break;
			case 12: deviceType = "DoorController";break;
			case 13: deviceType = "SoundSensor";break;
			case 14: deviceType = "MiButton";break;
			default : deviceType = "Unknow device";
  		}
		char str[200]={0};
		char str_url[200]={0};
		sprintf(str,"{\"address\":\"%d\",\"deviceType\":\"%s\",\"resourceSum\":\"%d\",\"linkQuality\":\"%d\"}",id,deviceType,rx[8],rx[len-1]);
		sprintf(str_url,"10.28.1.28:%d/device/API/deviceReg",PORT_CLIENT);
		curl_easy_setopt(posturl, CURLOPT_URL, str_url);
		curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
		curl_easy_perform(posturl);
		printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
		if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		{
			fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
			  fclose(sp);
		}
		
		//MXJ_SendRegisterMessage( id, MXJ_REGISTER_OK);

    break;
	}

    case MXJ_SEND_STATE:
	{
		char str[200]={0};
		char strtemp[200]={0};
		char str_url[200]={0};
		int i=0;
		sprintf(str,"{\"address\":\"%d\",\"linkQuality\":\"%d\",\"status\":[",id,rx[len-1]);
		for(i=0;i<rx[7];i++)
		{
			sprintf(strtemp,"{\"value\":\"%d\"}",rx[8+i]);
			strcat (str,strtemp);
			if(i!=rx[7]-1&&rx[7]>1)
			{
				strcat (str,",");
			}
		}
		strcat (str,"]}");
		
		sprintf(str_url,"10.28.1.28:%d/device/API/feedback/status",PORT_CLIENT);
		curl_easy_setopt(posturl, CURLOPT_URL, str_url);
		curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
		curl_easy_perform(posturl);
		printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
		if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		{
			fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
			  fclose(sp);
		}	
	break;
	}


    case MXJ_SENSOR_DATA:
	{
		char* type;
		uint16_t tempdata=0;
		tempdata=rx[10];
		tempdata <<= 8;
		tempdata |= rx[11];
		switch(rx[9])
		{
			case 0: type = "Temperature";break;
			case 1: type = "Humidity";break;
			case 2: type = "PM2.5";break;
			case 3: type = "PM10";break;
			case 4: type = "Sound";break;
			case 5: type = "Light";break;
			case 6: type = "Smoke";break;
			default : type = "Unknow type";
		}
		char str[200]={0};
		char str_url[200]={0};
		sprintf(str,"{\"address\":\"%d\",\"indaddressex\":\"%d\",\"data\":\"%d\",\"type\":\"%s\",\"linkQuality\":\"%d\"}",id,rx[8],tempdata,type,rx[len-1]);
		sprintf(str_url,"10.28.1.28:%d/device/API/command",PORT_CLIENT);
		curl_easy_setopt(posturl, CURLOPT_URL, str_url);
		curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
		curl_easy_perform(posturl);	
		
		printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
		if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		{
			fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
			  fclose(sp);
		}
						
	break;
	}


    case MXJ_PING_RESPONSE:
	{
		char str[200]={0};
		char str_url[200]={0};
		sprintf(str,"{\"address\":\"%d\",\"linkQuality\":\"%d\"}",id,rx[len-1]);
		sprintf(str_url,"10.28.1.28:%d/device/API/feedback/ping",PORT_CLIENT);
		curl_easy_setopt(posturl, CURLOPT_URL, str_url);
		curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
		curl_easy_perform(posturl);
		printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
		if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		{
			fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
			  fclose(sp);
		}    
    break;
	}

    case MXJ_DEVICE_ANNCE:
	{
		char str[200]={0};
		char str_url[200]={0};
		sprintf(str,"{\"address\":\"%d\",\"macAddr\":\"%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\"}",id,rx[11],rx[10],rx[9],rx[8],rx[7],rx[6],rx[5],rx[4]);
		sprintf(str_url,"10.28.1.28:%d/device/API/report",PORT_CLIENT);
		curl_easy_setopt(posturl, CURLOPT_URL, str_url);
		curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
		curl_easy_perform(posturl);
		printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
		if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		{
			fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
			  fclose(sp);
		}

    break;
	}	  
	
	case MXJ_SEND_RESET:
	{
		char str[200]={0};
		char str_url[200]={0};
		sprintf(str,"{\"address\":\"%d\"}",id);
		sprintf(str_url,"10.28.1.28:%d/device/API/rest",PORT_CLIENT);
		curl_easy_setopt(posturl, CURLOPT_URL, str_url);
		curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
		curl_easy_perform(posturl);
		printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
		if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		{
			fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
			  fclose(sp);
		}		
    break;
	} 

		
    case MXJ_XIAOMI18:	

	if(cid == 6)
	{
		if(len != 13 + 1)break;
		//printf("control up - find id = %d\n",i);
		//printf("id:%4x\n",id);
		if(rx[11] == 0x20)
		{
			//printf("double kick\n");
			char str[200]={0};
			char str_url[200]={0};
			sprintf(str,"{\"address\":\"%d\",\"indaddressex\":\"%d\",\"event\":\"%s\",\"linkQuality\":\"%d\"}",id,1,"DoubleClick",rx[len-1]);
			sprintf(str_url,"10.28.1.28:%d/device/API/command",PORT_CLIENT);
			curl_easy_setopt(posturl, CURLOPT_URL, str_url);
			curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
			curl_easy_perform(posturl);	
			printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
			if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
			{
				fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
				  fclose(sp);
			}			
		}
		else
		{
			//printf("action = %d\n",rx[12]);			
			char* event;
			switch(rx[12])
			{
				case 0: event = "PressDown";break;
				case 1: event = "PressUp";break;
				case 2: event = "DoubleClick";break;
				case 3: event = "Click";break;
				case 4: event = "BodyMove";break;
				case 5: event = "ReportData";break;
				case 6: event = "LongPress";break;
				case 7: event = "TriggerByCloud";break; 
				default : event = "Unknow event";
			}
			char str[200]={0};
			char str_url[200]={0};
			sprintf(str,"{\"address\":\"%d\",\"indaddressex\":\"%d\",\"event\":\"%s\",\"linkQuality\":\"%d\"}",id,1,event,rx[len-1]);
			sprintf(str_url,"10.28.1.28:%d/device/API/command",PORT_CLIENT);
			curl_easy_setopt(posturl, CURLOPT_URL, str_url);
			curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
			curl_easy_perform(posturl); 
			printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
			if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
			{
				fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
				  fclose(sp);
			}

		}
	}
	else if(cid == 0x406)
	{
		if(len != 13 + 1)break;
		//printf("control up - find id = %d\n",i);
		//printf("id:%4x\n",id);
		//printf("human detected\n");
		char str[200]={0};
		char str_url[200]={0};
		sprintf(str,"{\"address\":\"%d\",\"indaddressex\":\"%d\",\"event\":\"%s\",\"linkQuality\":\"%d\"}",id,1,"BodyMove",rx[len-1]);
		sprintf(str_url,"10.28.1.28:%d/device/API/command",PORT_CLIENT);
		curl_easy_setopt(posturl, CURLOPT_URL, str_url);
		curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
		curl_easy_perform(posturl);	
		printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
		if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		{
			fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
			  fclose(sp);
		}		
	}
	else if(cid == 0x402)
	{
		int16_t temp = 0;
		if(len != 14 + 1)break;
		static uint8_t temp_flag = 0;
		temp = rx[13];
		temp <<= 8;
		temp |= rx[12];
		//printf("temperature up - find id = %d\n",i);
		//printf("id:%4x\n",id);
		//printf("temperature = %02f\n",(float)temp/100);
		char str[200]={0};
		char str_url[200]={0};
		sprintf(str,"{\"address\":\"%d\",\"indaddressex\":\"%d\",\"type\":\"%s\",\"data\":\"%f\",\"linkQuality\":\"%d\"}",id,1,"Temperature",(float)temp/100,rx[len-1]);
		sprintf(str_url,"10.28.1.28:%d/device/API/value",PORT_CLIENT);
		curl_easy_setopt(posturl, CURLOPT_URL, str_url);
		curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
		curl_easy_perform(posturl);	
		printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
		if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		{
			fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
			  fclose(sp);
		}		

	}
	else if(cid == 0x405)
	{
	 	uint16_t temp = 0;
		if(len != 14 + 1)break;
		temp = rx[13];
		temp <<= 8;
		temp |= rx[12];
		//printf("humidity up - find id = %d\n",i);
		//printf("id:%4x\n",id);
		//printf("humidity = %02f\n",(float)temp/100);
		char str[200]={0};
		char str_url[200]={0};
		sprintf(str,"{\"address\":\"%d\",\"indaddressex\":\"%d\",\"type\":\"%s\",\"data\":\"%f\",\"linkQuality\":\"%d\"}",id,1,"Humidity",(float)temp/100,rx[len-1]);
		sprintf(str_url,"10.28.1.28:%d/device/API/value",PORT_CLIENT);
		curl_easy_setopt(posturl, CURLOPT_URL, str_url);
		curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
		curl_easy_perform(posturl);		
		printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
		if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
		{
			fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
			  fclose(sp);
		}		
	}
  	
	
	if(len>=27 && len < 32 + 1)
		{
			if(cid == 0 && rx[12] <= len - 13)
			{
				 uint8_t *temp_str = NULL;
				/*·????ú′?????*/
				temp_str = (uint8_t*)calloc(rx[12],sizeof(uint8_t));
				strncpy(temp_str, &rx[13],rx[12]);
				//printf("temp_str = %s\n",temp_str);
				if (0 == strcmp (temp_str, "lumi.sensor_switch"))
				{
					//printf("switch join\n");
					char str[200]={0};
					char str_url[200]={0};
					sprintf(str,"{\"address\":\"%d\",\"deviceType\":\"%s\",\"resourceSum\":\"%d\",\"linkQuality\":\"%d\"}",id,"MiButton",1,rx[len-1]);
					sprintf(str_url,"10.28.1.28:%d/device/API/deviceReg",PORT_CLIENT);
					curl_easy_setopt(posturl, CURLOPT_URL, str_url);
					curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
					curl_easy_perform(posturl);	
					printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
					if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
					{
						fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
						  fclose(sp);
					}					
				}
				else if (0 == strcmp (temp_str, "lumi.sensor_magnet"))
				{
					//printf("magnet join\n");
					char str[200]={0};
					char str_url[200]={0};
					sprintf(str,"{\"address\":\"%d\",\"deviceType\":\"%s\",\"resourceSum\":\"%d\",\"linkQuality\":\"%d\"}",id,"MagnetSensor",1,rx[len-1]);
					sprintf(str_url,"10.28.1.28:%d/device/API/deviceReg",PORT_CLIENT);
					curl_easy_setopt(posturl, CURLOPT_URL, str_url);
					curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
					curl_easy_perform(posturl);			
					printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
					if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
					{
						fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
						  fclose(sp);
					}					
				}
				else if (0 == strcmp (temp_str, "lumi.sensor_motion"))
				{
					//printf("motion join\n");
					char str[200]={0};
					char str_url[200]={0};
					sprintf(str,"{\"address\":\"%d\",\"deviceType\":\"%s\",\"resourceSum\":\"%d\",\"linkQuality\":\"%d\"}",id,"BodySensor",1,rx[len-1]);
					sprintf(str_url,"10.28.1.28:%d/device/API/deviceReg",PORT_CLIENT);
					curl_easy_setopt(posturl, CURLOPT_URL, str_url);
					curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
					curl_easy_perform(posturl);	
					printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
					if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
					{
						fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
						  fclose(sp);
					}					
				}
				else if (0 == strcmp (temp_str, "lumi.sensor_ht"))
				{
					//printf("ht join\n");
					char str[200]={0};
					char str_url[200]={0};
					sprintf(str,"{\"address\":\"%d\",\"deviceType\":\"%s\",\"resourceSum\":\"%d\",\"linkQuality\":\"%d\"}",id,"TemperatureSensor",1,rx[len-1]);
					sprintf(str_url,"10.28.1.28:%d/device/API/deviceReg",PORT_CLIENT);
					curl_easy_setopt(posturl, CURLOPT_URL, str_url);
					curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
					curl_easy_perform(posturl);	
					printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
					if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
					{
						fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
						  fclose(sp);
					}					
				}
			}
		}

		if(cid == 0 && len == 40 + 1)
		{
			char str[200]={0};
			char strtemp[200]={0};
			char str_url[200]={0};
			int i=0;
			sprintf(str,"{\"address\":\"%d\",\"linkQuality\":\"%d\"}",id,rx[len-1]);
			
			sprintf(str_url,"10.28.1.28:%d/device/API/feedback/status",PORT_CLIENT);
			curl_easy_setopt(posturl, CURLOPT_URL, str_url);
			curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
			curl_easy_perform(posturl);
			printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
			if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
			{
				fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
				  fclose(sp);
			}
		}



		
 	
    break;

	case MXJ_XIAOMI1C:
		if(cid == 0 && len == 35 + 1)
		{
			char str[200]={0};
			char strtemp[200]={0};
			char str_url[200]={0};
			int i=0;
			sprintf(str,"{\"address\":\"%d\",\"linkQuality\":\"%d\"}",id,rx[len-1]);
			
			sprintf(str_url,"10.28.1.28:%d/device/API/feedback/status",PORT_CLIENT);
			curl_easy_setopt(posturl, CURLOPT_URL, str_url);
			curl_easy_setopt(posturl, CURLOPT_POSTFIELDS,str);
			curl_easy_perform(posturl);
			printf("POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);
			if ((sp = fopen("/home/pi/zbclient/log.txt","a+")) != NULL)
			{
				fprintf(sp,"POST SEND:time=%d-%d-%d %d:%d:%d url=%s body=%s\n", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,str_url,str);			
				  fclose(sp);
			}
		}
	break;

    
  }
}

void thread(void)
{
	uint8_t ch = 0;
	uint8_t i = 0;
  uint8_t temp2=0;
  uint16_t id=0;
  
	while (1)
	{		
		if(read(usart_fd,&ch,1) == -1)
		{
			printf("usart read error\n");
			continue;
		}
   		//printf("recieve ch=%02x\n",ch);
		 switch (state)
    {
      case WW_STATE:
        if (ch == 0x55)
          state = AA_STATE;
          //printf("55");
        break;
        
      case AA_STATE:
        if (ch == 0xaa)
          state = CMD_STATE1;
        else
          state = WW_STATE;
        break;
        
      case CMD_STATE1:
        rxbuf[0] = ch;
        state = LEN_STATE;
        break;
        
      case LEN_STATE:
        LEN_Token = ch;

        tempDataLen = 0;     
          rxbuf[1] = LEN_Token;
          state = DATA_STATE;

        break;


      case DATA_STATE:

        // Fill in the buffer the first byte of the data 
        rxbuf[2 + tempDataLen++] = ch;

        // If number of bytes read is equal to data length, time to move on to FCS 
        if ( tempDataLen == LEN_Token - 1 )
            state = FCS_STATE;

        break;

      case FCS_STATE:

        FSC_Token = ch;
        uint8_t ii;
        uint8_t crcout;
        // Make sure it's correct 
        crcout = 0;
        for(ii=0;ii<1 + LEN_Token;ii++)
          crcout += rxbuf[ii];

        rxbuf[LEN_Token + 1] = crcout;
        if (crcout == FSC_Token)
        {
          //printf("recieve:");
          len_global = LEN_Token+1;
          for(ii=0;ii<LEN_Token+1;ii++)
            //printf("%02x",rxbuf[ii]);
            
          //printf("\n");
          id=rxbuf[2];
          id<<=8;
          id|=rxbuf[3];
          

          recieve_usart(rxbuf,LEN_Token+1);
          
          
          
        }
        else
        {
          // deallocate the msg 
          printf("crc wrong\n");
        }

        // Reset the state, send or discard the buffers at this point 
        state = WW_STATE;

        break;

      default:
        state = WW_STATE;
       break;
    }
	}

}
int SerialInit(void)
{
	int temp_fd=-1;
    temp_fd = open("/dev/ttyAMA0", O_RDWR|O_NOCTTY|O_NDELAY);
    if(-1 == temp_fd)
    {
        printf("Open Serial Port Error!\n");
        return -1;
    }
    if( (fcntl(temp_fd, F_SETFL, 0)) < 0 )
    {
        printf("Fcntl F_SETFL Error!\n");
        return -1;
    }
    if(tcgetattr(temp_fd, &stOld) != 0)
    {
        printf("tcgetattr error!\n");
        return -1;
    }
    stNew = stOld;
    cfmakeraw(&stNew);//将终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
    //set speed
    cfsetispeed(&stNew, 115200);//115200
    //cfsetospeed(&stNew, BAUDRATE);
    //set databits
    stNew.c_cflag |= (CLOCAL|CREAD);
    stNew.c_cflag &= ~CSIZE;
    stNew.c_cflag |= CS8;
    //set parity
    stNew.c_cflag &= ~PARENB;
    stNew.c_iflag &= ~INPCK;
    //set stopbits
    stNew.c_cflag &= ~CSTOPB;
    stNew.c_cc[VTIME]=0;    //指定所要读取字符的最小数量
    stNew.c_cc[VMIN]=1; //指定读取第一个字符的等待时间，时间的单位为n*100ms
                //如果设置VTIME=0，则无字符输入时read（）操作无限期的阻塞
    tcflush(temp_fd,TCIFLUSH);  //清空终端未完成的输入/输出请求及数据。
    if( tcsetattr(temp_fd,TCSANOW,&stNew) != 0 )
    {
        perror("tcsetattr Error!\n");
        return -1;
    }
    return temp_fd;
}

int main(void)
{
	pthread_t id;
    pthread_t send_usart_pr; 
  struct MHD_Daemon *daemon;
  uint8_t i=0;
  
  
	if(wiringPiSetup() < 0)
	{
		printf("wiringpi setup failed!\n");
		return 1;
	}

	if((usart_fd = SerialInit()) ==-1)
	{
		printf("serial open failed!\n");
		return 1;
	}
	printf("serial test start ...\n");

	/*
	if((usart_fd = serialOpen("/dev/ttyAMA0",115200)) < 0)
	{
		printf("serial open failed!\n");
		return 1;
	}
	printf("serial test start ...\n");
*/
	//ret=pthread_create(&id,NULL,(void *) thread,NULL);
	if(pthread_create(&id,NULL,(void *) thread,NULL)!=0)
	{
		printf ("Create pthread error!\n");
		return (1);
	}
	
   //printf("start server ...\n");

if(pthread_create(&send_usart_pr,NULL,(void *) thread_send,NULL)!=0)
	{
		printf ("Create pthread error!\n");
		return (1);
	}
	
  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                             &answer_to_connection, NULL,
                             MHD_OPTION_NOTIFY_COMPLETED, request_completed,
                             NULL, MHD_OPTION_END);
  if (NULL == daemon)
  {
    //printf("server creat failed!\n");
    return 1;
  }

  posturl = curl_easy_init();
  struct curl_slist *list = NULL;

	if(posturl){

		list = curl_slist_append(list, "Content-Type: application/json");
		curl_easy_setopt(posturl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(posturl, CURLOPT_URL, "10.28.1.28:10080/api/devices");
		curl_easy_setopt(posturl, CURLOPT_HTTPPOST, 1L);
		curl_easy_setopt(posturl, CURLOPT_WRITEFUNCTION, writer);
		//curl_easy_setopt(posturl, CURLOPT_WRITEDATA, &headerStr);
	}
	else
	   return (1);
  
   pinMode (24, OUTPUT);
   pinMode (27, OUTPUT);
   pinMode (25, INPUT) ;
   pinMode (22, OUTPUT);

  //
  //MXJ_GetIdxMessage( 0xe768 );
  //sleep(1);
  //MXJ_GetIdxMessage( 0x968f );
  //sleep(1);

  	digitalWrite(24,HIGH);

  	digitalWrite(22,HIGH);
	
  	usleep(500000);
	static int flag_led = 1;
	while(1)
	{  
		//usleep(500000);
		
		//digitalWrite(22,HIGH);
		sleep(30);
		//digitalWrite(22,LOW);
		usleep(500000);
		
	}
	

	//serialClose(usart_fd);
	return (0);
}

/*********************************************************************
 * @fn      MXJ_SendCtrlMessage
 *
 * @brief   Send the ctrl message to endpoint.
 *
* @param   ID
 *
 * @return  none
 */
void MXJ_SendCtrlMessage( uint16_t id ,uint8_t len,uint8_t msg1 , uint8_t msg2 , uint8_t msg3 )
{
//  uint8_t data[5]={MXJ_CTRL_DOWN,3,msg1,msg2,msg3};//è?a????1‰??°??
  uint8_t data[7]={0,len+3,(uint8_t)(id>>8),(uint8_t)id,msg1,msg2,msg3};
  send_usart(data,4+len);
}

/*********************************************************************
 * @fn      MXJ_SendRegisterMessage
 *
 * @brief   Send the register_request message to endpoint.
 *
 * @param   none
 *
 * @return  none
 
      //uint8_t data[7]={0,6,0x17,0x1f,1,1,1};//?????üá??y
     //uint8_t data[4]={10,3,0x17,0x1f};//?a1????????t??è??y
     //uint8_t data[12]={8,11,0x17,0x1f,0,0,0x7b,0x4f,1,2,2,1};//?a1????????téè???y    
     //send_usart(data,12);
 */
void MXJ_SendRegisterMessage( uint16_t id, uint8_t state )
{
  uint8_t data[5]={0,4,(uint8_t)(id>>8),(uint8_t)id,1};
  if(state == MXJ_REGISTER_OK)
  {
    data[0]=3;
    data[4]=1;
    send_usart(data,5);
  }
  else if(state == MXJ_REGISTER_FAILED)
  {
    data[0]=4;
    data[4]=0;
    send_usart(data,5);
  }
}

void MXJ_SendPingMessage( uint16_t id )
{
  uint8_t data[4]={0x0f,3,(uint8_t)(id>>8),(uint8_t)id};//è?a????1‰??°??
  send_usart(data,4);
}

void MXJ_GetIdxMessage( uint16_t id )
{
  uint8_t data[4]={0x0b,3,(uint8_t)(id>>8),(uint8_t)id};//è?a????1‰??°??
  send_usart(data,4);
}

/*********************************************************************
 * @fn      MXJ_GetStateMessage
 *
 * @brief   Get the state message from gateway.
 *
 * @param   none
 *
 * @return  none
 */
void MXJ_GetStateMessage( uint16_t id )
{
  uint8_t data[4]={5,3,(uint8_t)(id>>8),(uint8_t)id};//×??¨ò?êy?Y
  send_usart(data,4);
}

