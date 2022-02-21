  
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

struct customAttribute{
  std::string attributesTitle;
  std::string attributesValue; 
};

struct FileInformation{
  std::string itemID;
  std::string revID;
  std::string Name;
  std::string Descrption; 
  std::vector<customAttribute> attributes;
};

int login();
std::string getUserInput(std::string inputmessage);
tag_t create_Item(FileInformation fileINfo);
bool is_number(const std::string& s);
bool validateInputs(std::string ID, std::string RevID);
std::vector<FileInformation> readFile();  
std::vector<std::string> SpilitString(const std::string& str, const char& ch);
int askProcessingType();

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
			if(type == 1)
			{
				std::vector<FileInformation> fileInfos = readFile();
				for each (FileInformation var in fileInfos)
				{
					bool isvalid = validateInputs(var.itemID, var.revID);
					if(!isvalid) continue;
					create_Item(var);
				}
			}
			else if(type == 2)
			{

				FileInformation var;
				var.itemID = getUserInput("Please enter ItemId - Only Numeric values with five characters are allowed");  
				var.revID   = getUserInput("Please enter RevisionId - Only Numeric values with one character is allowed");
				var.Name   = getUserInput("Please enter Name - alpha Numeric charcters allowed");
				var.Descrption   = getUserInput("Please enter Description - alpha Numeric charcters allowed");

				std::cout << "ItemId   :" <<  var.itemID  << std::endl;
				std::cout << "RevisionId :" << 	var.revID  << std::endl;
				std::cout << "Name  :" << var.Name << std::endl;
				std::cout << "Description  :" << var.Descrption  << std::endl; 
	 
				bool isvalid = validateInputs(var.itemID, var.revID);
				if(!isvalid) return -1;   

				create_Item(var);
			} 

		std::cin.get();
		std::cin.get(); 
	return ITK_ok;
	 
}
int askProcessingType()
{
	std::cout << "Please select" << std::endl << "Read information from file -1" << std::endl 
			<< "provide information in cmd -2" << std::endl; 
	std::string selectedOption;
	std::cin >> selectedOption;

	if (selectedOption.length() !=1)
	{
		std::cout << "Provided option not available" << std::endl;
		return -1;
	}
	if(selectedOption =="1")
		return 1;
	else if(selectedOption =="2")
		return 2;
	else
		return -3; 
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

bool validateInputs(std::string ID, std::string RevID)
{ 
	if(!is_number(ID))
	{
			std::cout << "Provided Item Id has non numeric values" << std::endl;
			return false;
	}
	if(!is_number(RevID))
	{
			std::cout << "Provided Rev ID has non numeric values" << std::endl;
			return false;
	}
	if(ID.length()!= 5 )
	{
		std::cout << "Provided Item Id length is not 5" << std::endl;
			return false;
	}
	if(RevID.length() != 1 )
	{
		std::cout << "Provided Rev Id length is not 1" << std::endl;
			return false;
	} 

	tag_t* itemID= NULLTAG ; 
	const char* itemKey = "item_id";
	const char* itemvalue =  ID.c_str();
	int foundobjects;
	 
	ITEM_find_items_by_key_attributes(1, &itemKey, &itemvalue, &foundobjects, &itemID);  
	if(itemID == NULLTAG ) return true; // NO item available  
	int noOfIds;
	tag_t* RevisionIds;
	const char 
		*names[1] = { "item_id" },
		*values[1] = { ID.c_str() };
	ITEM_find_item_revs_by_key_attributes(1,names, values, RevID.c_str(), &noOfIds, &RevisionIds);
	//ITEM_find_revision(*itemID, writable, revID); // Deprecated

	 if(noOfIds == 0 ) return true; // NO item revison available 
	 else
	 {
		 std::cout << "Provided item id with Rev Id exsists" << std::endl;
			return false;
	 }
	 return false;
}

bool is_number(const std::string& s)
{ 
	return s.find_first_not_of(s) == std::string::npos;  
} 
std::vector<std::string> SpilitString(const std::string& str, const char& ch)
{
	std::string next;
	std::vector<std::string> result;

	// For each character in the string
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		// If we've hit the terminal character
		if (*it == ch) {
			// If we have some characters accumulated
			if (!next.empty()) {
				// Add them to the result vector
				result.push_back(next);
				next.clear();
			}
		} else {
			// Accumulate the next character into the sequence
			next += *it;
		}
	}
	if (!next.empty())
		 result.push_back(next);
	return result;
} 
tag_t create_Item(FileInformation fileINfo)
{
	Teamcenter::BusinessObjectRegistry& BORegInstance = Teamcenter::BusinessObjectRegistry::instance();
	Teamcenter::CreateInput* inputObj = dynamic_cast<Teamcenter::CreateInput* >(BORegInstance.createInputObject("Item", "Create"));
	Teamcenter::Item* item =dynamic_cast<Teamcenter::Item*>(BORegInstance.createBusinessObject(inputObj));
	int revID = stoi(fileINfo.revID);
	item->setItem_id(fileINfo.itemID);
	item->setObject_name(fileINfo.Name);
	item->setObject_desc(fileINfo.Descrption);
	item->setRevision_number(revID); 
	if(!fileINfo.attributes.empty() )
	{
		for each (customAttribute eachCustomAttribute in fileINfo.attributes)
		 {
			 item->setString(eachCustomAttribute.attributesTitle ,eachCustomAttribute.attributesValue, false);// Set Custom Attribute
		 }
	} 
	AOM_save_with_extensions(item->getTag()); //Saves file here in to Teamcenter
	return item->getTag(); 
}
std::vector<FileInformation> readFile()
{
	std::ifstream infile("Path of the file");
	std::string line; 
	std::vector<FileInformation> fileInfos ;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		if (line.rfind("#", 0) == 0) // comment Line
		{
		 continue;
		}
		if (line.find("|") != std::string::npos)  // contains attribute infos
		 {				 
			int pos = line.find_first_of("|");
			std::string secondhalf = line.substr(pos+1),
			firstHalf = line.substr(0, pos); 

			/*int pos1 = firstHalf.find_first_of(",");
			std::string RevID = firstHalf.substr(pos1+1),
			itemID = firstHalf.substr(0, pos1);*/   
			std::vector<std::string> ItemInfos = SpilitString(firstHalf, ',');

			FileInformation obj ;
			if(ItemInfos.size()>=0 && ItemInfos[0].length()!=0)obj.itemID =ItemInfos[0];
			if(ItemInfos.size()>=1 && ItemInfos[1].length()!=0)obj.revID =ItemInfos[1];
			if(ItemInfos.size()>=2 && ItemInfos[2].length()!=0)obj.Name =ItemInfos[2];
			if(ItemInfos.size()>=3 && ItemInfos[3].length()!=0)obj.Descrption =ItemInfos[3];
		
			std::vector<std::string> AttributeInfos = SpilitString(secondhalf, ',');
			std::vector<customAttribute> customAttributes ;
			for each (std::string eachAttributePair in AttributeInfos)
			{
				int pos3 = eachAttributePair.find_first_of("=");
				std::string secondhalf1   = eachAttributePair.substr(pos3+1),
				firstHalf1 = eachAttributePair.substr(0, pos3); 
				customAttribute customAttribute;
				customAttribute.attributesTitle = firstHalf1;
				customAttribute.attributesValue = secondhalf1;
				customAttributes.push_back(customAttribute);
			}
			obj.attributes = customAttributes;

			fileInfos.push_back(obj);
		 }  
		else // Contains only itemid and revison id
		{ 
			int pos1 = line.find_first_of(",");
			std::string  RevID = line.substr(pos1+1),
			itemID = line.substr(0, pos1);  
			FileInformation obj ;
			obj.itemID =itemID;
			obj.revID =RevID; 
			fileInfos.push_back(obj); 
		}
	}
	return fileInfos;
}
 
std::string getUserInput(std::string inputmessage )
{
	std::cout << inputmessage << std::endl;
	std::string value = "";
	 std::cin>>value;
	 
	return value;
} 
