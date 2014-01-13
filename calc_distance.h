/******************************************************************************
  File Name     : calc_distance.h
  Version       : 
  Author        : wangyang
  Created       : 2013/7/10
  Content       : calc_distance() and CStation implementation
  Description   : 经纬度距离计算
  History       : 

******************************************************************************/
#ifndef	  _CALC_DISTANCE_H_
#define	  _CALC_DISTANCE_H_

#include <iostream>
#include <fstream>
#include <iomanip.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>
#include <time.h>
/*
#include <sys/types.h>
#include <dirent.h>
*/
//using namespace std;

class CStation
{
public:	
	CStation(const CStation& cStation);
	CStation(){}
	~CStation(){}
	int		m_lac;
	int		m_ci;
	double	m_lon;
	double	m_lat;
	std::string m_type;
        std::string wlan_hot;
	CStation &operator=(const CStation& cStation);
	bool operator==(const CStation& cStation) const
	{
		return true;
	}
	
	int	get_lac() const
	{ return m_lac;	}
	int	get_ci() const
	{ return m_ci;	}
	double	get_lon() const
	{ return m_lon;	}
	double	get_lat() const
	{ return m_lat;	}
	std::string	get_type() const
	{ return m_type;	}
	
	void clear()
	{
		m_lac = 0;
		m_ci = 0;
		m_lon = 0;
		m_lat = 0;
		m_type.clear();
	}
// Set Function
	void set_lac(const int lac)
	{ m_lac = lac;	}	
  void set_ci(const int ci)
	{ m_ci = ci;	}	
	void set_lon(const double lon)
	{ m_lon = lon; }
  void set_lat(const double lat)
	{ m_lat = lat; }	
  void set_type(const  std::string &type)
	{ m_type = type;	}	
};

typedef std::vector<CStation> StationList;
std::ostream &operator<<( ostream &str, const StationList& listStation); 

#endif
