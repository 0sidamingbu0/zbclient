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
#define DOOR_IN_ID       	0X268c //Âà∑Âç°
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





typedef unsigned char   uint8_t;     //Êó†Á¨¶Âè∑8‰ΩçÊï∞
typedef unsigned short uint16_t;  


#define WW_STATE      0x55
#define AA_STATE      0xaa
#define CMD_STATE1     0x02
#define LEN_STATE      0x03
#define DATA_STATE     0x04
#define FCS_STATE      0x05

#define PORT            8888
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






int usart_fd;
  
uint8_t state = WW_STATE;
uint8_t  LEN_Token;
uint8_t  FSC_Token;
uint8_t  tempDataLen = 0;
uint8_t rxbuf[200];
uint8_t len_global=0;
int humand = 0;
int carded = 0;
int led_state = 0;


time_t now;
struct tm *tblock; 
int shefang = 0;
uint8_t flag_light1 = 0;
uint8_t flag_light2 = 0;

uint8_t flag_light21 = 0;
uint8_t flag_light22 = 0;
uint8_t flag_light23 = 0;
uint8_t flag_light24 = 0;
uint8_t flag_light25 = 0;
uint8_t flag_light26 = 0;

//uint8_t humaned = 0;
//CURL* geturl;

void send_usart(uint8_t *data,uint8_t len);
void MXJ_SendRegisterMessage( uint16_t , uint8_t );
void MXJ_SendPingMessage( uint16_t id );
void MXJ_GetIdxMessage( uint16_t id );

void MXJ_SendCtrlMessage( uint16_t ,uint8_t len, uint8_t , uint8_t , uint8_t );
void MXJ_SendStateMessage( uint16_t );

void MXJ_GetStateMessage( uint16_t id );
void del_dev(void);
int find_dev(int i);

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

char *str = "undefine";
char strxml[500];
int post_type;

char *json_str;
struct connection_info_struct
{
  int connectiontype;
  char *answerstring;
  struct MHD_PostProcessor *postprocessor;
};

const char *askpage = "{\"devices\":[{\"id\":10552,\"type\":7,\"control\":{\"devid\":1234,\"lights\":[0,1]},\"status\":[]},{\"id\":2155,\"type\":3,\"control\":{\"devid\":6445,\"lights\":[1,2]},\"status\":[]},{\"id\":12345,\"type\":7,\"control\":{},\"status\":[1,1,0]}]}";

const char *greetingpage =
  "<html><body><h1>Welcome, %s!</center></h1></body></html>";

const char *errorpage =
  "<html><body>This doesn't seem to be right.</body></html>";

const char *idolpage =
  "<html><body>hello idol</body></html>";

void build_json()
{
	int i=0;
  char str2[2000];
  strcpy(str2,"{\"devices\":[");
//	json_object *devices = json_object_new_array();
/*
	for(i=0;i<devsize;i++)
		{      
			json_object *state= json_object_new_array();
			json_object_array_add(state, json_object_new_int(mxj_device[i].state[0]));
			json_object_array_add(state, json_object_new_int(mxj_device[i].state[1]));
			json_object_array_add(state, json_object_new_int(mxj_device[i].state[2]));
      //printf("state = %s\n",json_object_to_json_string(state));
            
			
      //printf("control = %s\n",json_object_to_json_string(control));
            
			json_object *dev = json_object_new_object();
			json_object_object_add(dev, "id", json_object_new_int(mxj_device[i].id));
			json_object_object_add(dev, "type", json_object_new_int(mxj_device[i].type));				
			json_object_object_add(dev, "status", state);
			if(mxj_device[i].id == KETING_ID)
				json_object_object_add(dev, "name", json_object_new_string("KETING_ID"));
			if(mxj_device[i].id == CANTING_ID)
				json_object_object_add(dev, "name", json_object_new_string("CANTING_ID"));
			if(mxj_device[i].id == CHUFANG_ID)
				json_object_object_add(dev, "name", json_object_new_string("CHUFANG_ID"));
			if(mxj_device[i].id == MENTING_ID)
				json_object_object_add(dev, "name", json_object_new_string("MENTING_ID"));
			if(mxj_device[i].id == GUODAO_ID)
				json_object_object_add(dev, "name", json_object_new_string("GUODAO_ID"));
			if(mxj_device[i].id == ZHUWO_ID)
				json_object_object_add(dev, "name", json_object_new_string("ZHUWO_ID"));
			if(mxj_device[i].id == CIWO_ID)
				json_object_object_add(dev, "name", json_object_new_string("CIWO_ID"));
			
      //printf("dev = %s\n",json_object_to_json_string(dev));
      
			//json_object_array_add(devices, dev);
      //json_object *obj = json_object_array_get_idx(devices, i);
      
      
      //snprintf (str2, MAXANSWERSIZE, json_object_to_json_string(obj));
      strcat(str2,json_object_to_json_string(dev));
      if(i<devsize-1)
        strcat(str2,",");
      else
        strcat(str2,"]}");
      
      
      //printf("------------------build-json-----------\n");
	  
			if(state!=NULL)
				json_object_put(state);
			if(dev!=NULL)
				json_object_put(dev);

				
		}
   
   */

   //printf("str2 = %s\n",str2); 
   strcpy(json_str,str2);
   //printf("build json_str = %s\n",json_str); 
   //json_str=str2;
   
	
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
  printf ("====New %s request for %s using version %s\n", method, url, version);
 
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
		printf("get json_str = %s\n",json_str);
		if(json_str!=NULL)
			return send_page (connection,json_str);
		else
			return send_page (connection, errorpage);
    }
  
  //printf("method= %s\n", method);

  if (0 == strcmp (method, "POST"))
  	{  		
	  int ret;
	  if (*upload_data_size != 0)
	  {

		
		
		const char* length = MHD_lookup_connection_value (connection, MHD_HEADER_KIND, MHD_HTTP_HEADER_CONTENT_LENGTH); 	
			const char* body = MHD_lookup_connection_value (connection, MHD_POSTDATA_KIND, NULL);		
			printf("length=%s\n",length);
			printf("body=%s\n",body);
			printf("url=%s\n",url); 
			if(length != NULL && body != NULL)
			{		  
				len2 = atoi(length);
				re_body = (uint8_t*)calloc(len2,sizeof(uint8_t));
				strncpy(re_body,body,len2);
				printf("now datetime: %d-%d-%d %d:%d:%d - New %s request for %s using version %s - len2=%d re_body=\n%s\n End \n", tblock->tm_year, tblock->tm_mon, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,method, url, version,len2,re_body);
			}
			else if(body != NULL)
			{
				len2 = 0;
				post_type = 0;
				printf("now datetime: %d-%d-%d %d:%d:%d - New %s request for %s using version %s - len2=0 re_body=\nNULL\n End \n", tblock->tm_year, tblock->tm_mon, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,method, url, version);
			}
		
			if ((sp = fopen("/home/pi/zbclient/re.txt","a+")) != NULL)
			{
				fprintf(sp,"now datetime: %d-%d-%d %d:%d:%d - New %s request for %s using version %s - len2=%d re_body=\n%s\n End \n", tblock->tm_year, tblock->tm_mon, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec,method, url, version,len2,re_body);
				fclose(sp);
			}
		
			if(len2 != 0)
			{
				json_object *my_object = json_tokener_parse(re_body);
				if(json_object_to_json_string(my_object) != NULL && (0 != strcmp (json_object_to_json_string(my_object), "null")))
				{
					  uint16_t id=0;
					  uint8_t state11=0,state22=0,state33=0;
					  post_type = 1;
					  json_object *type = NULL;
					  json_object *name = NULL;
					  
					  json_object *devid = NULL;
					  json_object_object_get_ex(my_object, "devid",&devid);
					  json_object *state1 = NULL;
					  json_object_object_get_ex(my_object, "state1",&state1);
					  json_object *state2 = NULL;
					  json_object_object_get_ex(my_object, "state2",&state2);
					  json_object *state3 = NULL;
					  json_object_object_get_ex(my_object, "state3",&state3);
					  json_object_object_get_ex(my_object, "type",&type);
					  json_object_object_get_ex(my_object, "name",&name);
				  
					  id = (uint16_t)json_object_get_int(devid);
					  state11 = (uint16_t)json_object_get_int(state1);
					  state22 = (uint16_t)json_object_get_int(state2);
					  state33 = (uint16_t)json_object_get_int(state3);
				  
				  
				  
					 
				  
					  printf("\n");
					  //printf("state11 = %d\n", state11);
					  //printf("state22 = %d\n", state22);
					  //printf("state33 = %d\n", state33);
					  printf("recieve post json= %s\n", json_object_to_json_string(my_object));
				}
				
			}
			
		*upload_data_size = 0;
		return MHD_YES;
	  }
	  else if(post_type == 2)
	  	  return send_page (connection,strxml);
	  else if(post_type == 1&&json_str!=NULL)
		  return send_page (connection,json_str);
	  else
		  return send_page (connection, errorpage);  
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



void send_usart(uint8_t *data,uint8_t len) //id,state1,state2,state3 1=ø™,0=πÿ,2=±£≥÷
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
  printf("fifo_add %d:\n",len + 3);

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

		
		if(fifo_read(&txbuf,&len))
		{
			printf("send %d: ",fifo_start);

		  for(i=0;i<len;i++)
		  {
		    serialPutchar(usart_fd,txbuf[i]);
		    printf("%02x ",txbuf[i]);
		  }
		  printf("\n");
		}

		usleep(5000);
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
  int tempctrl[3]={2,2,2};
  digitalWrite(27,HIGH);
  led_state = 0;
  time(&now);
  tblock = localtime(&now);
  printf("\n");
  printf("recieved:%d - at: %d-%d-%d %d:%d:%d\n",len,tblock->tm_year, tblock->tm_mon, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);
  
  for(i=0;i<len;i++)
    printf("%02x ",rx[i]);
  printf("\n");
  printf("------------------------\n");
  
 
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
		
		if(id == SWITCH_OUTSIDE_ID) 
	   	{
	        printf("control up - find id = %d\n",i);
		    printf("id:%4x\n",id);
			if(rx[8] == 1 && rx[9]!=2)
			{				
				flag_light1 = rx[9];
				MXJ_SendCtrlMessage(LIGHT_INSIDE_ID,3,rx[9],2,2);
			}
			else if(rx[8] == 2 && rx[9]!=2)
			{
				flag_light2 = rx[9];
				MXJ_SendCtrlMessage(LIGHT_INSIDE_ID,3,2,2,rx[9]);
			}

	     }

				
		if(id == LIGHT_INSIDE_ID) 
	   	{
	        printf("control up - find id = %d\n",i);
		    printf("id:%4x\n",id);
			if(rx[8] == 1 && rx[9]!=2)
			{				
				flag_light1 = rx[9];
				MXJ_SendCtrlMessage(SWITCH_OUTSIDE_ID,3,rx[9],2,2);
			}
			else if(rx[8] == 2 && rx[9]!=2)
			{
				flag_light2 = rx[9];
				MXJ_SendCtrlMessage(SWITCH_OUTSIDE_ID,3,2,2,rx[9]);
			}
	     }

		if(id == DOOR_IN_ID) 
	   	{
	        printf("control up - find id = %d\n",i);
		    printf("id:%4x\n",id);
			if(rx[9]==0)
			{				
				if(carded <= 1)
				{
					MXJ_SendCtrlMessage(POWER_ID,1,1,1,1);
					carded = 10;
				}	
				if(carded > 1 )
					carded = 10;
			}
	     }



//////=============
	if(id == SWITCH2_ID_1) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			flag_light21 = rx[9];
			flag_light24 = rx[9];
			MXJ_SendCtrlMessage(SWITCH2_ID_4,3,rx[9],2,2);
		}
		else if(rx[8] == 2 && rx[9]!=2)
		{
			MXJ_SendCtrlMessage(SWITCH2_ID_4,3,2,2,rx[9]);
		}	
	 }
	if(id == SWITCH2_ID_2) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			flag_light22 = rx[9];
			flag_light25 = rx[9];
			MXJ_SendCtrlMessage(SWITCH2_ID_5,3,rx[9],2,2);
		}
		else if(rx[8] == 2 && rx[9]!=2)
		{
			MXJ_SendCtrlMessage(SWITCH2_ID_5,3,2,2,rx[9]);
		}
	 }
	if(id == SWITCH2_ID_3) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			flag_light23 = rx[9];
			flag_light26 = rx[9];
			MXJ_SendCtrlMessage(SWITCH2_ID_6,3,rx[9],2,2);
		}
		else if(rx[8] == 2 && rx[9]!=2)
		{
			MXJ_SendCtrlMessage(SWITCH2_ID_6,3,2,2,rx[9]);
		}
	 }
	if(id == SWITCH2_ID_4) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			flag_light24 = rx[9];
			flag_light21 = rx[9];
			MXJ_SendCtrlMessage(SWITCH2_ID_1,3,rx[9],2,2);
		}
		else if(rx[8] == 2 && rx[9]!=2)
		{
			MXJ_SendCtrlMessage(SWITCH2_ID_1,3,2,2,rx[9]);
		}
	 }
	if(id == SWITCH2_ID_5) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			flag_light25 = rx[9];
			flag_light22 = rx[9];
			MXJ_SendCtrlMessage(SWITCH2_ID_2,3,rx[9],2,2);
		}
		else if(rx[8] == 2 && rx[9]!=2)
		{
			MXJ_SendCtrlMessage(SWITCH2_ID_2,3,2,2,rx[9]);
		}
	 }
	if(id == SWITCH2_ID_6) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			flag_light26 = rx[9];
			flag_light23 = rx[9];
			MXJ_SendCtrlMessage(SWITCH2_ID_3,3,rx[9],2,2);
		}
		else if(rx[8] == 2 && rx[9]!=2)
		{
			MXJ_SendCtrlMessage(SWITCH2_ID_3,3,2,2,rx[9]);
		}
	 }

	if(id == SWITCHN1_ID_1) 
	{
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]==0)
		{				
			static int flag = 1;
			flag = 3-flag;
			MXJ_SendCtrlMessage(SWITCHN3_ID_1,3,flag-1,2,2);
			MXJ_SendCtrlMessage(SWITCHN1_ID_1,1,flag-1,2,2);
			MXJ_SendCtrlMessage(POWERN_ID_1,1,flag-1,2,2);
		}
	 }

	if(id == SWITCHN2_ID_1) 
	{
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]==0)
		{				
		
			static int flag1 = 1;
			flag1 = 3-flag1;
			MXJ_SendCtrlMessage(SWITCHN3_ID_1,3,2,flag1-1,2);
			MXJ_SendCtrlMessage(SWITCHN2_ID_1,2,flag1-1,2,2);
		}
		if(rx[8] == 2 && rx[9]==0)
		{				
			static int flag2 = 1;
			flag2 = 3-flag2;
			MXJ_SendCtrlMessage(SWITCHN3_ID_1,3,2,2,flag2-1);
			MXJ_SendCtrlMessage(SWITCHN2_ID_1,2,2,flag2-1,2);
		}
	 }
	if(id == SWITCHN3_ID_1) 
	{
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]==0)
		{				
			static int flag1 = 1;
			flag1 = 3-flag1;
			MXJ_SendCtrlMessage(SWITCHN1_ID_1,1,flag1-1,0,0);
			MXJ_SendCtrlMessage(SWITCHN3_ID_1,3,flag1-1,2,2);
		}
		if(rx[8] == 2 && rx[9]==0)
		{				
			static int flag2 = 1;
			flag2 = 3-flag2;
			MXJ_SendCtrlMessage(SWITCHN2_ID_1,2,flag2-1,2,0);
			MXJ_SendCtrlMessage(SWITCHN3_ID_1,3,2,flag2-1,2);
		}
		if(rx[8] == 3 && rx[9]==0)
		{				
			static int flag3 = 1;
			flag3 = 3-flag3;
			MXJ_SendCtrlMessage(SWITCHN2_ID_1,2,2,flag3-1,0);
			MXJ_SendCtrlMessage(SWITCHN3_ID_1,3,2,2,flag3-1);
		}
	 }



	if(id == SWITCH3_ID_3) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			MXJ_SendCtrlMessage(POWER_ID_1,1,rx[9],rx[9],rx[9]);
		}
	}
	if(id == SWITCH3_ID_4) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			MXJ_SendCtrlMessage(POWER_ID_2,1,rx[9],rx[9],rx[9]);
		}
	}
	if(id == SWITCH3_ID_5) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			MXJ_SendCtrlMessage(POWER_ID_3,1,rx[9],rx[9],rx[9]);
		}
	}
	if(id == SWITCH3_ID_6) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			MXJ_SendCtrlMessage(POWER_ID_4,1,rx[9],rx[9],rx[9]);
		}
	}
	if(id == SWITCH3_ID_7) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			MXJ_SendCtrlMessage(POWER_ID_5,1,rx[9],rx[9],rx[9]);
		}
	}
	if(id == SWITCH3_ID_8) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			MXJ_SendCtrlMessage(POWER_ID_6,1,rx[9],rx[9],rx[9]);
		}
	}
	if(id == SWITCH3_ID_9) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			MXJ_SendCtrlMessage(POWER_ID_7,1,rx[9],rx[9],rx[9]);
		}
	}
	if(id == SWITCH3_ID_10) 
	{
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[8] == 1 && rx[9]!=2)
		{				
			MXJ_SendCtrlMessage(POWER_ID_8,1,rx[9],rx[9],rx[9]);
		}
	}


//=========================
		
    break;
    
    case MXJ_REGISTER_REQUEST:
      MXJ_SendRegisterMessage(id,MXJ_REGISTER_OK);
    break;

    case MXJ_SEND_STATE:
		
    break;

    case MXJ_SENSOR_DATA:
      //MXJ_GetStateMessage(id);
    break;

    case MXJ_GET_STATE:
		MXJ_SendCtrlMessage(id,3,2,2,2);
    break;

	case MXJ_SEND_RESPONSE:
		
    break;	
		
    case MXJ_XIAOMI18:	

	if(cid == 6)
	{
		if(len != 13)break;
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		if(rx[11] == 0x20)
		{
			printf("double kick\n");
			if(id == XMKG_ENTER_ID)
			{
				MXJ_SendCtrlMessage(SWITCH_ALAM_ID,3,0,0,0);
				shefang = 0;
			}

			if(id == XMKG_M_ID || id == XMKG_X_ID)
			{
				MXJ_SendCtrlMessage(LIGHT_INSIDE_ID,3,0,0,0);
				MXJ_SendCtrlMessage(SWITCH_OUTSIDE_ID,3,0,0,0);
				flag_light1 = 0;
				flag_light2 = 0;
			}

			if(id==XMKG_DOOR_ID)//0=PRESSED
			{								
				MXJ_SendCtrlMessage(DOOR_OUT_ID,3,1,1,1);			
			}

		}
		else
		{
			printf("action = %d\n",rx[12]);
			if(id==XMKG_M_ID&&rx[12] == 1)//0=PRESSED
			{				
				flag_light1 = ! flag_light1;
				MXJ_SendCtrlMessage(LIGHT_INSIDE_ID,3,flag_light1,2,2);
				MXJ_SendCtrlMessage(SWITCH_OUTSIDE_ID,3,flag_light1,2,2);
				
			}
			
			if(id==XMKG_X_ID&&rx[12] == 1)
			{
				flag_light2 = ! flag_light2;
				MXJ_SendCtrlMessage(LIGHT_INSIDE_ID,3,2,2,flag_light2);
				MXJ_SendCtrlMessage(SWITCH_OUTSIDE_ID,3,2,2,flag_light2);
				
			}
			if(id==XMKG_ENTER_ID&&rx[12] == 1)
			{
				MXJ_SendCtrlMessage(SWITCH_ALAM_ID,3,1,0,0);
				shefang = 1;
			}

			
			if(id==XMKG_DOOR_ID)//0=PRESSED
			{								
				MXJ_SendCtrlMessage(DOOR_OUT_ID,3,rx[12],rx[12],rx[12]);			
			}
		
			
			if(id==XIAOMIMENCI_ID)
			{
				if(shefang == 0)
				{
					MXJ_SendCtrlMessage(LIGHT_INSIDE_ID,3,rx[12],rx[12],rx[12]);
					MXJ_SendCtrlMessage(SWITCH_OUTSIDE_ID,3,rx[12],rx[12],rx[12]);
					flag_light1 = rx[12];
					flag_light2 = rx[12];

				}
				else
				{
					MXJ_SendCtrlMessage(SWITCH_ALAM_ID,3,2,2,1);						
				}
			}
//================			
			if(id==XIAOMIKAIGUAN1_ID&&rx[12] == 1)//0=PRESSED
			{				
				flag_light21 = ! flag_light21;
				MXJ_SendCtrlMessage(SWITCH2_ID_1,3,flag_light21,2,2);				
			}
			if(id==XIAOMIKAIGUAN2_ID&&rx[12] == 1)//0=PRESSED
			{				
				flag_light22 = ! flag_light22;
				MXJ_SendCtrlMessage(SWITCH2_ID_2,3,flag_light22,2,2);				
			}
			if(id==XIAOMIKAIGUAN3_ID&&rx[12] == 1)//0=PRESSED
			{				
				flag_light23 = ! flag_light23;
				MXJ_SendCtrlMessage(SWITCH2_ID_3,3,flag_light23,2,2);				
			}
			if(id==XIAOMIKAIGUAN4_ID&&rx[12] == 1)//0=PRESSED
			{				
				flag_light24 = ! flag_light24;
				MXJ_SendCtrlMessage(SWITCH2_ID_4,3,flag_light24,2,2);				
			}
			if(id==XIAOMIKAIGUAN5_ID&&rx[12] == 1)//0=PRESSED
			{				
				flag_light25 = ! flag_light25;
				MXJ_SendCtrlMessage(SWITCH2_ID_5,3,flag_light25,2,2);				
			}
			if(id==XIAOMIKAIGUAN6_ID&&rx[12] == 1)//0=PRESSED
			{				
				flag_light26 = ! flag_light26;
				MXJ_SendCtrlMessage(SWITCH2_ID_6,3,flag_light26,2,2);				
			}

			if(id==XIAOMIMENCI1_ID)//0=PRESSED
			{				
				MXJ_SendCtrlMessage(SWITCH3_ID_1,3,rx[12],rx[12],rx[12]); 			
			}
			if(id==XIAOMIMENCI2_ID)//0=PRESSED
			{				
				MXJ_SendCtrlMessage(SWITCH3_ID_2,3,rx[12],rx[12],rx[12]); 			
			}

////=====================
		}
	}
	else if(cid == 0x406)
	{
		if(len != 13)break;
		printf("control up - find id = %d\n",i);
		printf("id:%4x\n",id);
		printf("human detected\n");
		if(id == XIAOMIRENTI_ID)
		{			
			if(humand <= 1)
			{
				MXJ_SendCtrlMessage(LIGHT_OUTSIDE_ID,3,1,2,2);
				humand = 10;
			}	
			if(humand > 1 )
				humand = 10;
		}
	}
	else if(cid == 0x402)
	{
		int16_t temp = 0;
		if(len != 14)break;
		static uint8_t temp_flag = 0;
		temp = rx[13];
		temp <<= 8;
		temp |= rx[12];
		printf("temperature up - find id = %d\n",i);
		printf("id:%4x\n",id);
		printf("temperature = %02f\n",(float)temp/100);
		
		if(temp > 3600 && temp_flag != 1)
		{
			MXJ_SendCtrlMessage(POWER_ID,1,1,2,2);
			temp_flag = 1;
		}
		if(temp < 3500 && temp_flag != 2)
		{
			MXJ_SendCtrlMessage(POWER_ID,1,0,2,2);
			temp_flag = 2;
		}
		
		

	}
	else if(cid == 0x405)
	{
	 	uint16_t temp = 0;
		if(len != 14)break;
		temp = rx[13];
		temp <<= 8;
		temp |= rx[12];
		printf("humidity up - find id = %d\n",i);
		printf("id:%4x\n",id);
		printf("humidity = %02f\n",(float)temp/100);
	}
  	
	
	if(len>=27 && len < 32)
		{
			if(cid == 0 && rx[12] <= len - 13)
			{
				 uint8_t *temp_str = NULL;
				/*∑÷≈‰ƒ⁄¥Êø’º‰*/
				temp_str = (uint8_t*)calloc(rx[12],sizeof(uint8_t));
				/*Ω´hello–¥»Î*/
				strncpy(temp_str, &rx[13],rx[12]);
				printf("temp_str = %s\n",temp_str);
				if (0 == strcmp (temp_str, "lumi.sensor_switch"))
				{
					printf("switch join\n");
					//char str[200]={0};
					//sprintf(str,"type=register_request&dev_id=%d&dev_type=11&dev_string=xiaomikaiguan&idxs=1",id);
					//curl_easy_setopt(geturl, CURLOPT_POSTFIELDS,str);
					//curl_easy_perform(geturl);
	
				}
				else if (0 == strcmp (temp_str, "lumi.sensor_magnet"))
				{
					printf("magnet join\n");
					//char str[200]={0};
					//sprintf(str,"type=register_request&dev_id=%d&dev_type=10&dev_string=xiaomimenci&idxs=1",id);
					//curl_easy_setopt(geturl, CURLOPT_POSTFIELDS,str);
	
					//curl_easy_perform(geturl);			
				}
				else if (0 == strcmp (temp_str, "lumi.sensor_motion"))
				{
					printf("motion join\n");
					//char str[200]={0};
					//sprintf(str,"type=register_request&dev_id=%d&dev_type=3&dev_string=xiaomihuman&idxs=1",id);
					//curl_easy_setopt(geturl, CURLOPT_POSTFIELDS,str);
	
					//curl_easy_perform(geturl);			
				}
				else if (0 == strcmp (temp_str, "lumi.sensor_ht"))
				{
					printf("ht join\n");
					//char str[200]={0};
					//sprintf(str,"type=register_request&dev_id=%d&dev_type=9&dev_string=xiaomiwenshidu&idxs=1",id);
					//curl_easy_setopt(geturl, CURLOPT_POSTFIELDS,str);
					//curl_easy_perform(geturl);			
				}
			}
		}



		
 	
    break;

	case MXJ_XIAOMI1C:

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
		ch = serialGetchar(usart_fd);
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

int main(void)
{
	pthread_t id;
    pthread_t send_usart_pr; 
  struct MHD_Daemon *daemon;
  uint8_t i=0;
  

  json_str = (char*)calloc(2000,sizeof(char));
  
  
	if(wiringPiSetup() < 0)
	{
		printf("wiringpi setup failed!\n");
		return 1;
	}
	if((usart_fd = serialOpen("/dev/ttyAMA0",115200)) < 0)
	{
		printf("serial open failed!\n");
		return 1;
	}
	printf("serial test start ...\n");

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
   pinMode (24, OUTPUT);
   pinMode (27, OUTPUT);
   pinMode (25, INPUT) ;
  int tt=0;
  int j=0,k=0;
  //
  //MXJ_GetIdxMessage( 0xe768 );
  //sleep(1);
  //MXJ_GetIdxMessage( 0x968f );
  //sleep(1);

  
  build_json();
	static int flag_led = 1;
	while(1)
	{  
		//usleep(500000);
		sleep(1);
		
		if(humand == 1)
		{
			MXJ_SendCtrlMessage(LIGHT_OUTSIDE_ID,3,0,2,2);			
		}
		if(humand > 0)
		{	
			printf("humand = %d\n",humand);
			humand--;
		}

		if(carded == 1)
		{
			MXJ_SendCtrlMessage(POWER_ID,1,0,0,0);	
		}
		if(carded > 0)
		{	
			printf("carded = %d\n",carded);
			carded--;
		}
	}
	

	serialClose(usart_fd);
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
//  uint8_t data[5]={MXJ_CTRL_DOWN,3,msg1,msg2,msg3};//Ëá™ÂÆö‰πâÊï∞ÊçÆ
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
 
      //uint8_t data[7]={0,6,0x17,0x1f,1,1,1};//øÿ÷∆√¸¡Ó°˝
     //uint8_t data[4]={10,3,0x17,0x1f};//ø™πÿ≈‰÷√Œƒº˛ªÒ»°°˝
     //uint8_t data[12]={8,11,0x17,0x1f,0,0,0x7b,0x4f,1,2,2,1};//ø™πÿ≈‰÷√Œƒº˛…Ë÷√°˝    
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
  uint8_t data[4]={0x0f,3,(uint8_t)(id>>8),(uint8_t)id};//Ëá™ÂÆö‰πâÊï∞ÊçÆ
  send_usart(data,4);
}

void MXJ_GetIdxMessage( uint16_t id )
{
  uint8_t data[4]={0x0b,3,(uint8_t)(id>>8),(uint8_t)id};//Ëá™ÂÆö‰πâÊï∞ÊçÆ
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
  uint8_t data[4]={5,3,(uint8_t)(id>>8),(uint8_t)id};//◊‘∂®“Â ˝æ›
  send_usart(data,4);
}

