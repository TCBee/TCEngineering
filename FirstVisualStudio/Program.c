#include <stdio.h>
#include <string.h>

#include<tcinit/tcinit.h>
#include<tc/tc_macros.h>
#include<tccore/item.h>
#include<tccore/grm.h>
#include<tccore/workspaceobject.h>
#include<ae/dataset.h>

// constants for maximum number of lines in the file and max length of a line
#define MAX_LINES 100
#define MAX_LEN 1000

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

char* OUTPUTLOCATION;

char Lines[MAX_LINES][MAX_LEN];

int readTextFile(char*);
int processEachItem(char*);
int processEachRev(tag_t);
/*
 This program read itemid and look for those itemid pdf data
 -f=file_path -u=userid -p=password -g=group -o=outputPathforPDFDataset(C:/Temp/)

 -u=infodba -p=infodba -g=dba  -o=C:/Temp/ -f=C:/Temp/testfile.txt
*/
int ITK_user_main(int argc, char** argv)
{
	int retCode = 0;

	char* inputFilePath = ITK_ask_cli_argument("-f=");
	char* userId = ITK_ask_cli_argument("-u=");
	char* password = ITK_ask_cli_argument("-p=");
	char* grp = ITK_ask_cli_argument("-g=");
	OUTPUTLOCATION = ITK_ask_cli_argument("-o=");

	//char* filePath = "C:/Users/stuff/Desktop/LC/testfile.txt";
	retCode = readTextFile(inputFilePath);
	if (retCode != 0)
	{
		printf("read Failed \n");
		return -1;
	}
	else
	{
		printf("Read passed \n");
	}

	size_t n = sizeof(Lines) / sizeof(Lines[0]);
	printf("Itemids count : %d \n", n);

	if (n == 0)
		return 0;

	ITKCALL(ITK_init_module(userId, password, grp));

	for (int i = 0; i < n; i++)
	{
		char* itemId = Lines[i];
		printf("Itemid : %s \n", itemId);
		if (strlen(itemId) == 0) continue;

		retCode = processEachItem(itemId);
		if (retCode != 0)
			printf("couldn't Process item id %s\n", itemId);
	}
	return 0;
}

int processEachItem(char* itemId)
{
	tag_t itemID = NULLTAG;
	tag_t itemLatestRev = NULLTAG;
	int returnCode = 0;
	ITKCALL(ITEM_find_item(itemId, &itemID));

	if (itemID == NULLTAG)
	{
		printf("Itemid : %s not found \n", itemId);
		return -1;
	}

	ITKCALL(ITEM_ask_latest_rev(itemID, &itemLatestRev));
	if (itemLatestRev == NULLTAG)
	{
		printf("Itemid : %s latest rev not found \n", itemId);
		return -1;
	}
	returnCode = processEachRev(itemLatestRev);

	return returnCode;
}

int readTextFile(char* filePath)
{
	FILE *fptr = NULL;
	int i = 0;
	int tot = 0;

	fptr = fopen(filePath, "r");
	if (fptr == NULL)
	{
		printf("Error opening file.\n");
		return 1;
	}

	while (!feof(fptr) && !ferror(fptr))
	{
		if (fgets(Lines[i], MAX_LEN, fptr) != NULL)
		{
			i++;
		}
	}
	tot = i;
	return 0;
}
int processEachRev(tag_t itemRev)
{
	int sec_Obj_Count = 0;
	tag_t secObjTag = NULLTAG;
	GRM_relation_t* secObjs;
	char* objectType;

	int counter = 0;

	tag_t* primaryObj;
	WSO_description_t WSO_des;

	/*int refCount = 0;
	tag_t* refObjs;*/

	char filename[500];

	ITKCALL(GRM_list_secondary_objects(itemRev, NULLTAG, &sec_Obj_Count, &secObjs));
	if (sec_Obj_Count == 0)
	{
		printf("ERR : Couldn't find secondary attached object \n");
		return -1;
	}

	for (int i = 0; i < sec_Obj_Count; i++)
	{
		secObjTag = secObjs[i].secondary;
		primaryObj = secObjs[i].primary;
		ITKCALL(WSOM_ask_object_type2(secObjTag, &objectType));
		printf("Secondary Object Type : %s \n", objectType);

		if (strcmp(objectType, "PDF") != 0)
		{
			MEM_free(objectType);
			continue;
		}
		else
		{
			ITKCALL(WSOM_describe(primaryObj, &WSO_des));
			snprintf(filename, sizeof(filename), "%s%s%d.pdf", OUTPUTLOCATION, WSO_des.object_name, counter++);

			printf("OP Location : %s\n", filename);
			if (access(filename, F_OK) != 0)
			{
				ITKCALL(AE_export_named_ref(secObjTag, "PDF_Reference", filename));
			}
			else
			{
				printf("File %s already exsists application skips above exporting", filename);
			}
		}
		MEM_free(objectType);
	}
	MEM_free(secObjs);
	return 0;
}
