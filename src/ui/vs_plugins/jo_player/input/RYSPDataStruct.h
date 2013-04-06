#pragma once
#pragma pack(show)		//��ʾ��ǰpacking aligment���ֽ���
#pragma pack(push)		//�������״̬
#pragma pack(4)			//�趨Ϊ4�ֽڶ���	

//cmd
#define RYSP_OPEN_STREAM		0x07        //��ʼԤ��
#define RYSP_OPEN_STREAM_RET	0x08        //Ԥ��Ӧ��

#define RYSP_OPEN_FILE			0x0F        //��ʼ�ط�
#define RYSP_OPEN_FILE_RET		0x10        //�ط�Ӧ��
#define RYSP_REQ_DATA			0x11		//��������
#define	RYSP_REQ_DATA_RET		0x12		//Ӧ��

//JOSP ����Э��ͷ 
struct RYSP_CtrlHead 
{ 
	unsigned char start_code[4];    //ħ���ֶ� JOSP
	unsigned char version:4;        //�汾��
	unsigned char type:2;           //Э������ 0-tcp, 1-udp
	unsigned char flag:2;           //�ְ���־ 0-δ�ְ�, 1-��ʼ, 2-�м�, 3-������
	unsigned char cmd;              //����
	unsigned short sq_num;          //�����
	unsigned char ret;              //������ͷ 0-�޴���
	unsigned char reserved;
	unsigned short ex_length;       //��չ���ݳ���
	unsigned char user_id[32];      //�û�ID
	unsigned char crc[4];           //У����(ѭ������У��)
};

//RYSP ����Ƶ  
struct RYSP_OpenData 
{ 
	char res_id[32];		//��ԴID
	int stream_type;		//�������� 0-������,1-������
}; 

//RYSP ����Ƶ���� 
struct RYSP_OpenRetData 
{ 
	unsigned char media_code[4];	//ý������ JOMS
	//��Ƶ��Ϣ
	unsigned char iframe_interval;	//I֡���
	unsigned char fps;				//֡��
	unsigned short width;
	unsigned short height;

	//��Ƶ��Ϣ
}; 

//RYSP ����ý������ 
struct RYSP_RequestData 
{ 
	long long time_stamp;		//��ʼʱ��(0-��Ч)
	int time_interval;			//ʱ����(ms),��Ƶ�ж೤
}; 

//RYSP ����ý������Ӧ�� 
struct RYSP_RequestRetData
{ 
	int ret;					//������(0-�ɹ�)
}; 


//RYSP ����ͷ��ʽ 
struct RYSP_DataHead 
{ 
	unsigned char start_code[4];	//ħ���ֶ� JOAV
	unsigned int data_len;		//���ݳ���(������ͷ)
	unsigned int frame_type;		//3-I֡,4-B֡,5-P֡,6-��Ƶ֡
	long long time_stamp;			//ʱ���
	unsigned long frame_seq;		//֡���к�
	int b_last_frame;				//BOOL����,������־
}; 

#pragma pack(pop)				//�ָ�����״̬