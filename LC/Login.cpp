  
//TC Reference
#include<tcinit\tcinit.h> 
#include<metaframework\BusinessObjectRegistry.hxx>
#include<tccore\Item.hxx>
#include<tccore\aom.h>
#include<tccore\item.h> 
// C/CPP refrence
#include<stdio.h>
#include<iostream>
#include<string>
#include<sstream> 
#include <fstream> 
#include <string> 

int login(); 
int ITK_user_main(int argc, char* argv[])
{
	/*try
	{*/
		int ifail = ITK_ok;  

		int type = askProcessingType();
	
		// Code to login to Teamcenter.
		ifail = login();
		if (ifail != ITK_ok)
		{
			return -1; 
		}   
	return ITK_ok; 
}
 
int login()
{
	int ifail = ITK_ok;   
	#pragma region
	ITK_init_to_login();
	ifail =	ITK_init_module("userid", "PAssword", "group");
	if(ifail == ITK_ok)
	{
		std::cout<< " able to login"<< std::endl;
		return ITK_ok;
	}
	else
	{
		std::cout<< "Cant able to login"<< std::endl;	
		return -1;
	}
	#pragma endregion Log Code. 
} 
 
