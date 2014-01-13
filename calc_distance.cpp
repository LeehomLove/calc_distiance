/******************************************************************************
  File Name     : calc_distance.cpp
  Version       : 
  Author        : wangyang
  Created       : 2013/7/10
  Content       : calc_distance() and Station implementation
  Description   : ��γ�Ⱦ������,���������С��20��
  History       : 

******************************************************************************/
#include "calc_distance.h"
#include <math.h>
using namespace std;

double radian(double d);  
double get_distance(double lat1, double lng1, double lat2, double lng2);  
void getNextPound(string &theString, string &substring,string token);
void calc_distance(const char *file_1 ,const char *file_2,const char *file_3,const char* file_4,const char *file_5,const char* file_6 );
//string &readpath(string &str);

typedef struct  {
	int		m_lac;
	int		m_ci;
	double	m_lon;
	double	m_lat;
	char  m_type[20];
	//string m_type;
}Pos;

//�洢������С��20����������鶨��
typedef struct {
				double distance;
				Pos pos[20];
}minstation;
		
// �󻡶�  
double radian(double d)  
{  
    return d *  3.1415926 / 180.0;   //�Ƕȡ� = �� / 180  
}

//���㾭γ�Ⱦ���  

double get_distance(double lat1, double lng1, double lat2, double lng2)  
{  
    double radLat1 = radian(lat1);  
    double radLat2 = radian(lat2);  

    double a = radLat1 - radLat2;  
    double b = radian(lng1) - radian(lng2);  

    double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2) )));  
    dst = dst * 6378137;  
    //dst= round(dst * 10000) / 10000;  
    return dst;  
}  

//theString�е��ַ�������token�ͽ�token֮ǰ���ַ���������������substring��
void getNextPound(string &theString, string &substring,string token)
{
    size_t pos ;
    if(theString.size()<=0)
    {
      substring.clear();
      return;
    }
    pos= theString.find(token);
    if((pos==string::npos) && (theString.size()>0))
    {
     substring=theString;
     theString.clear();
     return;
    }
    else
    {
     substring.assign(theString, 0, pos);
     theString.assign(theString, pos + 1, theString.size());
    }
}

/*
//�������ļ������������ļ���
string &readpath(string &str)
{
	char line[1024]={0};
	char result[1024]={0}; 
	string p;
	DIR *dirptr = 0;
    struct dirent *entry = 0;
	
	if((dirptr = opendir(str.c_str())) == 0)
	{
		printf("open dir failed!\n");
	}
	else
	{
		while(entry = readdir(dirptr))
		{
			strcpy(line,entry->d_name);
		}
		closedir(dirptr);
	}
	sprintf(result ,"%s/%s",str.c_str(),line);
	str = result;
	return str;
	//strcat(str ,);
}
*/

//���㾭γ�Ⱦ�����С��20��
void calc_distance(const char *file_1 ,const char *file_2,const char *file_3,const char* file_4,const char *file_5,const char* file_6 )
{
//��������
	//int		m_lac;
	int		m_ci;
	double	m_lon;
	double	m_lat;
	double   m_distance;
	int min_distance;
	int i,j;	
	int flag=0;
	int flag_2= 0;
	string str,SrcStr,DestStr;
	short   skip_flag= 0;;	
	vector<CStation>::iterator Tstation_1;
	vector<CStation>::iterator Tstation_2;

	minstation one_minstation;
	minstation two_minstation;
	//minstation three_minstation;
	
		
//����װ�����ݵ�����	
	StationList listGstation;
	listGstation.clear();
	StationList listTstation;
	listTstation.clear();
	StationList listRstation;
	listRstation.clear();
	
	
//���������,�������ļ���·��
/*	ifstream file("path.txt",ios::in);
	//ifstream afs;
	int fla = 0;
	while(!file.eof())
	{
		fla++;
		getline(file,SrcStr);
		string p = readpath(SrcStr);
		if (fla == 1)
		{
			std::cout<<p<<std::endl;
			ofstream afs(p.c_str(),ios::out|ios::app);
		}
		elif (fla == 2)
		{
			std::cout<<p<<std::endl;
			ofstream bfs(p.c_str(),ios::out|ios::app);
		}
		elif (fla == 3)
		{
			std::cout<<p<<std::endl;
			ofstream cfs(p.c_str(),ios::out|ios::app);
		}
		elif (fla == 4)
		{
			std::cout<<p<<std::endl;
			ifstream efs(p.c_str(),ios::out|ios::app);
		}
		elif (fla == 5)
		{
			std::cout<<p<<std::endl;
			ifstream ffs(p.c_str(),ios::out|ios::app);
		}		elif (fla == 2)
		{
			std::cout<<p<<std::endl;
			ifstream gfs(p.c_str(),ios::out|ios::app);
		}
		elif (fla == 3)
		{
			std::cout<<p<<std::endl;
			ofstream cfs(p.c_str(),ios::out|ios::app);
		}
		//else
	}
	*/
	
//�����	
	ofstream afs(file_4,std::ios::out);
	ofstream bfs(file_5,std::ios::out);
	ofstream cfs(file_6,std::ios::out);
//������
	ifstream efs(file_1,std::ios::in);
	ifstream ffs(file_2,std::ios::in);
	ifstream gfs(file_3,std::ios::in);	

//ȡÿһ���ļ�

	//try
	//{
		CStation Gstation;
		while ( !efs.eof() )//��һ�ű�
		{
			getline(efs,SrcStr);
			//���Ե�һ�б���
			skip_flag++;
			if(skip_flag == 1 )continue ;
			if(SrcStr.empty()) continue ;
			//iffs >> m_id>>m_lac>>m_ci>>m_lon>>m_lat>>m_distance
			
			Gstation.clear();
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
			Gstation.set_lac(atoi(DestStr.c_str()));
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
			Gstation.set_ci(atoi(DestStr.c_str()));
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
			Gstation.set_lon(atof(DestStr.c_str()));
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
			Gstation.set_lat(atof(DestStr.c_str()));
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
			Gstation.set_type(DestStr.c_str());
			
			listGstation.push_back(Gstation) ;
		}
		efs.close() ;
		std::cout<<"Gstation num is "<<listGstation.size()<< std::endl;
		
		skip_flag = 0;	
		CStation Tstation;		
		while ( !ffs.eof() )//�ڶ��ű�
		{
			getline(ffs,SrcStr);
			//���Ե�һ�б���
			skip_flag++;
			if(skip_flag == 1 )continue ;
			if(SrcStr.empty())continue ;
			
			Tstation.clear();
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
				Tstation.set_lac(atoi(DestStr.c_str()));
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
				Tstation.set_ci(atoi(DestStr.c_str()));
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
				Tstation.set_lon(atof(DestStr.c_str()));
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
				Tstation.set_lat(atof(DestStr.c_str()));
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
				Tstation.set_type(DestStr.c_str());

			listTstation.push_back(Tstation) ;
		}
		ffs.close() ;
		std::cout<<"Tstation num is "<<listTstation.size()<< std::endl;

		skip_flag= 0;
		CStation Rstation;
		while ( !gfs.eof() )//�����ű�
		{
			getline(gfs,SrcStr);
			//���Ե�һ�б���
			skip_flag++;
			if(skip_flag == 1 )continue ;
			if(SrcStr.empty())continue ;
			
			Rstation.clear();
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
				Rstation.set_lon(atof(DestStr.c_str()));
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
				Rstation.set_lat(atof(DestStr.c_str()));
			getNextPound(SrcStr, DestStr,  ",");
			if(DestStr.empty())continue ;
				Rstation.set_type(DestStr.c_str());
				//if(DestStr == "��վ" || )
			listRstation.push_back(Rstation) ;
		}
		gfs.close() ;
		std::cout<<"Rstation num is "<<listRstation.size()<< std::endl;


		for(Tstation_1=listGstation.begin();Tstation_1 != listGstation.end();Tstation_1++)
		{
			flag = 0;
			flag_2 = 0;
			memset(&one_minstation, 0, sizeof(minstation));
			memset(&two_minstation, 0, sizeof(minstation));
			//memset(&three_minstation, 0, sizeof(minstation));
			
			for(Tstation_2 = listTstation.begin();Tstation_2 != listTstation.end();Tstation_2++)
			{
					if(Tstation_2->get_type() == "��վ" /*|| Tstation_1->get_type() == "��վ"*/)
					{
							//flag = 0;
							m_distance = get_distance(Tstation_1->get_lat(), Tstation_1->get_lon(),Tstation_2->get_lat(), Tstation_2->get_lon());  
							
							if(one_minstation.distance == 0 )
							{
								one_minstation.distance = m_distance;
								one_minstation.pos[flag].m_lac = Tstation_2->get_lac();
								one_minstation.pos[flag].m_ci = Tstation_2->get_ci();
								one_minstation.pos[flag].m_lon = Tstation_2->get_lon();
								one_minstation.pos[flag].m_lat = Tstation_2->get_lat();
								//string����ֱ�Ӹ����ַ���
								strcpy(one_minstation.pos[flag].m_type ,Tstation_2->get_type().c_str());

								flag++;
							}
							else if(one_minstation.distance > m_distance )
							{
								memset(&one_minstation, 0, sizeof(minstation));
								flag = 0;
								one_minstation.distance = m_distance;
								one_minstation.pos[flag].m_lac = Tstation_2->get_lac();
								one_minstation.pos[flag].m_ci = Tstation_2->get_ci();
								one_minstation.pos[flag].m_lon = Tstation_2->get_lon();
								one_minstation.pos[flag].m_lat = Tstation_2->get_lat();
								strcpy(one_minstation.pos[flag].m_type ,Tstation_2->get_type().c_str());
								flag++;
							}
							else if(one_minstation.distance == m_distance )
							{
								if(flag >= 20)
									continue;
								//ֱ�����ṹ�������룬����ṹ�����ˣ�continue;
								one_minstation.distance = m_distance;
								one_minstation.pos[flag].m_lac = Tstation_2->get_lac();
								one_minstation.pos[flag].m_ci = Tstation_2->get_ci();
								one_minstation.pos[flag].m_lon = Tstation_2->get_lon();
								one_minstation.pos[flag].m_lat = Tstation_2->get_lat();
								strcpy(one_minstation.pos[flag].m_type ,Tstation_2->get_type().c_str());
								flag++;
								/******************/
							}
							
					}
					else if( Tstation_2->get_type() == "���ڷֲ�"||Tstation_2->get_type() == "���ڸ���")
					{
						//output(&one_minstation, &Tstation_1,&Tstation_2);
						//flag = 0;
						m_distance = get_distance(Tstation_1->get_lat(), Tstation_1->get_lon(),Tstation_2->get_lat(), Tstation_2->get_lon());  
						
						if(two_minstation.distance == 0 )
						{
							two_minstation.distance = m_distance;
							two_minstation.pos[flag_2].m_lac = Tstation_2->get_lac();
							two_minstation.pos[flag_2].m_ci = Tstation_2->get_ci();
							two_minstation.pos[flag_2].m_lon = Tstation_2->get_lon();
							two_minstation.pos[flag_2].m_lat = Tstation_2->get_lat();
							//string����ֱ�Ӹ����ַ���
							strcpy(two_minstation.pos[flag_2].m_type ,Tstation_2->get_type().c_str());

							flag_2++;
						}
						else if(two_minstation.distance > m_distance )
						{
							memset(&two_minstation, 0, sizeof(minstation));
							flag_2 = 0;
							two_minstation.distance = m_distance;
							two_minstation.pos[flag_2].m_lac = Tstation_2->get_lac();
							two_minstation.pos[flag_2].m_ci = Tstation_2->get_ci();
							two_minstation.pos[flag_2].m_lon = Tstation_2->get_lon();
							two_minstation.pos[flag_2].m_lat = Tstation_2->get_lat();
							strcpy(two_minstation.pos[flag_2].m_type ,Tstation_2->get_type().c_str());
							flag_2++;
							}
						else if(two_minstation.distance == m_distance )
						{
							if(flag_2 >= 20)
								continue;
							//ֱ�����ṹ�������룬����ṹ�����ˣ�continue;
							two_minstation.distance = m_distance;
							two_minstation.pos[flag_2].m_lac = Tstation_2->get_lac();
							two_minstation.pos[flag_2].m_ci = Tstation_2->get_ci();
							two_minstation.pos[flag_2].m_lon = Tstation_2->get_lon();
							two_minstation.pos[flag_2].m_lat = Tstation_2->get_lat();
							strcpy(two_minstation.pos[flag_2].m_type ,Tstation_2->get_type().c_str());
							flag_2++;
							/******************/
						}
							
					}
			}
			for (int x= 0; x < flag; x++)
                         {
                              afs<<Tstation_1->get_lac()<<"-"
                                 <<Tstation_1->get_ci()<<","
                                 <<Tstation_1->get_type()<<","
                                 <<Tstation_1->get_lon()<<","
                                 <<Tstation_1->get_lat()<<","
                                 <<one_minstation.pos[x].m_lac<<"-"
                                 <<one_minstation.pos[x].m_ci<<","
                                 <<one_minstation.pos[x].m_type<<","
                                 <<one_minstation.pos[x].m_lon<<","
                                 <<one_minstation.pos[x].m_lat<<","
                                 <<one_minstation.distance<<endl;
                                   //�����һ����¼����¼��Ϣ����(a cal_id a.ά����Ϣ,B�ṹ����Ϣ
                          }
                          for (int x= 0; x < flag_2; x++)
                           {
                                  afs<<Tstation_1->get_lac()<<"-"
                                    <<Tstation_1->get_ci()<<","
                                    <<Tstation_1->get_type()<<","
                                    <<Tstation_1->get_lon()<<","
                                    <<Tstation_1->get_lat()<<","
                                    <<two_minstation.pos[x].m_lac<<"-"
                                    <<two_minstation.pos[x].m_ci<<","
                                    <<two_minstation.pos[x].m_type<<","
                                    <<two_minstation.pos[x].m_lon<<","
                                    <<two_minstation.pos[x].m_lat<<","
                                    <<two_minstation.distance<<endl;
                                    //�����һ����¼����¼��Ϣ����(a cal_id a.ά����Ϣ,B�ṹ����Ϣ
                           }
		}
		afs.close();
		
		
		for(Tstation_1=listGstation.begin();Tstation_1 != listGstation.end();Tstation_1++)
		{
			flag = 0;
			memset(&one_minstation, 0, sizeof(minstation));

			for(Tstation_2=listRstation.begin();Tstation_2 != listRstation.end();Tstation_2++)
			{
				m_distance = get_distance(Tstation_1->get_lat(), Tstation_1->get_lon(),Tstation_2->get_lat(), Tstation_2->get_lon());  
				
				if(one_minstation.distance == 0 )
				{
					one_minstation.distance = m_distance;
					//one_minstation.pos[flag].m_lac = Tstation_2->get_lac();
					//one_minstation.pos[flag].m_ci = Tstation_2->get_ci();
					one_minstation.pos[flag].m_lon = Tstation_2->get_lon();
					one_minstation.pos[flag].m_lat = Tstation_2->get_lat();
					//string����ֱ�Ӹ����ַ���
					strcpy(one_minstation.pos[flag].m_type ,Tstation_2->get_type().c_str());

					flag++;
				}
				else if(one_minstation.distance > m_distance )
				{
					memset(&one_minstation, 0, sizeof(minstation));
					flag = 0;
					one_minstation.distance = m_distance;
					//one_minstation.pos[flag].m_lac = Tstation_2->get_lac();
					//one_minstation.pos[flag].m_ci = Tstation_2->get_ci();
					one_minstation.pos[flag].m_lon = Tstation_2->get_lon();
					one_minstation.pos[flag].m_lat = Tstation_2->get_lat();
					strcpy(one_minstation.pos[flag].m_type ,Tstation_2->get_type().c_str());
					flag++;
				}
				else if(one_minstation.distance == m_distance )
				{
					if(flag >= 20)
						continue;
					//ֱ�����ṹ�������룬����ṹ�����ˣ�continue;
					one_minstation.distance = m_distance;
					//one_minstation.pos[flag].m_lac = Tstation_2->get_lac();
					//one_minstation.pos[flag].m_ci = Tstation_2->get_ci();
					one_minstation.pos[flag].m_lon = Tstation_2->get_lon();
					one_minstation.pos[flag].m_lat = Tstation_2->get_lat();
					strcpy(one_minstation.pos[flag].m_type ,Tstation_2->get_type().c_str());
					flag++;
					/******************/
				}

			}
			
			for (int x= 0; x < flag; x++)
			{
				bfs<<Tstation_1->get_lac()<<"-"
					<<Tstation_1->get_ci()<<","
					<<Tstation_1->get_type()<<","
					<<Tstation_1->get_lon()<<","
					<<Tstation_1->get_lat()<<","
					<<one_minstation.pos[x].m_type<<","
					<<one_minstation.pos[x].m_lon<<","
					<<one_minstation.pos[x].m_lat<<","
					<<one_minstation.distance<<endl;
			//�����һ����¼����¼��Ϣ����(a cal_id a.ά����Ϣ,B�ṹ����Ϣ
			}
			
		}
		bfs.close();


		for(Tstation_1=listTstation.begin();Tstation_1 != listTstation.end();Tstation_1++)
		{
			flag = 0;
			memset(&one_minstation, 0, sizeof(minstation));

			for(Tstation_2=listRstation.begin();Tstation_2 != listRstation.end();Tstation_2++)
			{
				//flag = 0;
				m_distance = get_distance(Tstation_1->get_lat(), Tstation_1->get_lon(),Tstation_2->get_lat(), Tstation_2->get_lon());  
				
				if(one_minstation.distance == 0 )
				{
					one_minstation.distance = m_distance;
					//one_minstation.pos[flag].m_lac = Tstation_2->get_lac();
					//one_minstation.pos[flag].m_ci = Tstation_2->get_ci();
					one_minstation.pos[flag].m_lon = Tstation_2->get_lon();
					one_minstation.pos[flag].m_lat = Tstation_2->get_lat();
					//string����ֱ�Ӹ����ַ���
					strcpy(one_minstation.pos[flag].m_type ,Tstation_2->get_type().c_str());

					flag++;
				}
				else if(one_minstation.distance > m_distance )
				{
					memset(&one_minstation, 0, sizeof(minstation));
					flag = 0;
					one_minstation.distance = m_distance;
					//one_minstation.pos[flag].m_lac = Tstation_2->get_lac();
					//one_minstation.pos[flag].m_ci = Tstation_2->get_ci();
					one_minstation.pos[flag].m_lon = Tstation_2->get_lon();
					one_minstation.pos[flag].m_lat = Tstation_2->get_lat();
					strcpy(one_minstation.pos[flag].m_type ,Tstation_2->get_type().c_str());
					flag++;
				}
				else if(one_minstation.distance == m_distance )
				{
					if(flag >= 20)
						continue;
					//ֱ�����ṹ�������룬����ṹ�����ˣ�continue;
					one_minstation.distance = m_distance;
					//one_minstation.pos[flag].m_lac = Tstation_2->get_lac();
					//one_minstation.pos[flag].m_ci = Tstation_2->get_ci();
					one_minstation.pos[flag].m_lon = Tstation_2->get_lon();
					one_minstation.pos[flag].m_lat = Tstation_2->get_lat();
					strcpy(one_minstation.pos[flag].m_type ,Tstation_2->get_type().c_str());
					flag++;
					/******************/
				}

			}
			
			for (int x= 0; x < flag; x++)
			{
				cfs<<Tstation_1->get_lac()<<"-"
					<<Tstation_1->get_ci()<<","
					<<Tstation_1->get_type()<<","
					<<Tstation_1->get_lon()<<","
					<<Tstation_1->get_lat()<<","
					<<one_minstation.pos[x].m_type<<","
					<<one_minstation.pos[x].m_lon<<","
					<<one_minstation.pos[x].m_lat<<","
					<<one_minstation.distance<<endl;
			//�����һ����¼����¼��Ϣ����(a cal_id a.ά����Ϣ,B�ṹ����Ϣ
			}
			
		}
		cfs.close();	
	
	//}
	
	//catch (exception &p)//?
	//{
	//	cout<<"Execption:"<<endl;
	//}
		
}

CStation::CStation(const CStation& cStation)
{
	m_lac = cStation.m_lac;
	m_ci = cStation.m_ci;
	m_lon = cStation.m_lon;
	m_lat = cStation.m_lat;
	m_type = cStation.m_type;

}

CStation& CStation::operator=(const CStation& cStation)
{
//����Ը�ֵ
	if( this != &cStation )
	{
		m_lac = cStation.m_lac;
		m_ci = cStation.m_ci;
		m_lon = cStation.m_lon;
		m_lat = cStation.m_lat;
		m_type = cStation.m_type;
	}
	return *this;
}

//������
int main (int argc, const char * argv[])  
{
	//CStation cstation;
	if(argc < 6 ) 
	{
		std::cout<<"argc is not enough!!!\n"
		<<"��һ������ΪGSM��\n"
		<<"�ڶ�������ΪTD��\n"
		<<"����������ΪWLAN��\n"
		<<"���ĸ�����ΪGSM&TD��\n"
		<<"���������ΪGSM&WLAN��\n"
		<<"����������ΪTD&WLAN��"<<endl;
		exit(1);
	}
	calc_distance(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
    return 0;  

} 


