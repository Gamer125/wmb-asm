#define DLLMAIN
#ifndef BUILDING_DLL
#define BUILDING_DLL
#endif

#include "..\..\SDK\include\wmb_asm_sdk_plugin.h"

#undef DLLIMPORT
#define DLLIMPORT __declspec (dllexport)

int stage = SDK_STAGE_BEACON;

sAsmSDK_Config *CONFIG = NULL;
bool *DEBUG = NULL;
FILE **Log = NULL;

bool DidInit = 0;

#ifdef __cplusplus
  extern "C" {
#endif

DLLIMPORT char *GetStatus(int *error_code)
{
    if(stage==SDK_STAGE_AUTH && !nds_data->finished_first_assembly)
    {
        *error_code=SDK_STAGE_AUTH;
        return (char*)"02: Failed to find the Authentication packet; Maybe the receiving DS never attempted to download?\n";
    }
    if(stage==SDK_STAGE_RSA)
    {
        *error_code=SDK_STAGE_RSA;
        return (char*)"03: Failed to find the RSA frame, try doing the capture again?\n";
    }
    if(stage==SDK_STAGE_HEADER)
    {
        *error_code=SDK_STAGE_HEADER;
        return (char*)"04: Failed to find the header. Major bugged capture! Try capturing again.\n";
    }
    if(stage==SDK_STAGE_DATA)
    {
        char *str;
        int missed=0;
        int found=0;
        str=(char*)malloc(256);
        memset(str, 0, 256);
        *error_code=SDK_STAGE_DATA;

            if(nds_data!=NULL)
            {

                if(nds_data->data_sizes != NULL)
                {
                    for(int i=0; i<nds_data->arm7e_seq; i++)
                    {
                        if(nds_data->data_sizes[i]==0)
                        {
                            missed++;
                        }
                        else
                        {
                            found++;
                        }
                    }
                }

                sprintf(str,"05: Failed to find all the necessary data. Missed %d packets, got %d out of %d packets. %d percent done.\n",missed,found,nds_data->arm7e_seq,(int)GetPrecentageCompleteAsm());
            }

        return str;
    }

	*error_code=-1;
	return NULL;
}

DLLIMPORT int QueryFailure()
{
    if(stage>=SDK_STAGE_DATA)return 2;
    if(stage<=SDK_STAGE_HEADER)return 1;

    return 0;
}

DLLIMPORT bool Handle802_11(unsigned char *data, int length)
{
     /*if(stage==SDK_STAGE_BEACON)return WMBProcessBeacons(data,length);
     if(stage==SDK_STAGE_AUTH)return WMBProcessAuth(data,length);
     if(stage==SDK_STAGE_RSA)return WMBProcessRSA(data,length);
     if(stage==SDK_STAGE_HEADER)return WMBProcessHeader(data,length);
     if(stage==SDK_STAGE_DATA)return WMBProcessData(data,length);*/

     return 0;
}

DLLIMPORT void Reset(sAsmSDK_Config *config)
{
    if(!DidInit)
    {
        //printf("B\n");
        ResetAsm = config->ResetAsm;
        //printf("ga n dat %p config %p\n", nds_data, CONFIG->nds_data);
        nds_data = config->nds_data;
        DEBUG = config->DEBUG;
        Log = config->Log;
        //printf("C\n");
        CONFIG = config;
        //printf("D\n");
        DidInit = 1;
        //printf("E\n");


    }

    stage=SDK_STAGE_BEACON;
    /*last_seq=0;

    memset(host_mac,0,6);
	memset(client_mac,0,6);*/
}

#ifdef __cplusplus
  }
#endif
