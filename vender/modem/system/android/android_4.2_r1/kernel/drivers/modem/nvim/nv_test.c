 /*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*lint -save -e537*/
#include "bsp_nvim.h"
#include "nv_comm.h"
#include "nv_file.h"
#include "nv_xml_dec.h"
#include "bsp_hardtimer.h"
/*lint -restore +e537*/


u32 nv_kernel_init_test(void);
u32 nv_remain_init_test(void);
u32 nv_read_part_test(u32 nvid, u32 off, u32 len);
u32 nv_read_rand_test(u32 nvid);
u32 nv_get_len_test01(void);
u32 nv_get_len_test02(void);
u32 nv_read_test_01(void);
u32 nv_read_test_02(void);
u32 nv_read_test_03(void);
u32 nv_read_test_04(void);
u32 nv_read_test_05(void);
u32 nv_read_test_06(void);
u32 nv_read_test_07(void);
u32 nv_write_test01(void);
u32 nv_write_test02(void);
u32 nv_write_test03(void);
u32 nv_write_test04(void);
u32 nv_write_test05(void);
u32 nv_backup_test(void);
u32 nv_revert_test(void);
u32 nv_revert_manuf_version_test(void);


u32 nv_kernel_init_test(void)
{
    u32 ret = NV_ERROR;
    struct nv_global_ddr_info_stru* ddr_info = (struct nv_global_ddr_info_stru*)NV_GLOBAL_INFO_ADDR;

    if(ddr_info->acore_init_state == NV_INIT_OK)
    {
        return NV_OK;
    }

    ret = (u32)bsp_nvm_kernel_init();

    return ret;

}

u32 nv_remain_init_test(void)
{
    u32 ret = NV_ERROR;
    struct nv_global_ddr_info_stru* ddr_info = (struct nv_global_ddr_info_stru*)NV_GLOBAL_INFO_ADDR;

    if(ddr_info->acore_init_state == NV_INIT_OK)
    {
        return NV_OK;
    }

    ret = (u32)bsp_nvm_remain_init();

    return ret;

}


u32 nv_read_part_test(u32 nvid,u32 off,u32 len)
{
    u32 ret;
    u8* tempdata;
    u32 i= 0;

    tempdata = (u8*)nv_malloc(len +1);
    if(NULL == tempdata)
    {
        return BSP_ERR_NV_MALLOC_FAIL;
    }

    ret = bsp_nvm_readpart(nvid,off,tempdata,len);
    if(NV_OK != ret)
    {
        nv_free(tempdata);
        return BSP_ERR_NV_READ_DATA_FAIL;
    }

    for(i=0;i<len;i++)
    {
        if((i%16) == 0)
        {
            printf("\n");
        }
        printf("%2.2x ",(u8)(*(tempdata+i)));
    }

    printf("\n");
    nv_free(tempdata);
    return NV_OK;
}

u32 nv_get_len_test01(void)
{
    u32 ret;
    u32 len = 0;
    u32 itemid = 0xd007;

    ret = bsp_nvm_get_len(itemid,&len);
    return ret;
}

u32 nv_get_len_test02(void)
{
    u32 ret= NV_ERROR;
    u32 len = 0;
    u32 itemid = 0xffff;

    ret = bsp_nvm_get_len(itemid,&len);
    if(ret == BSP_ERR_NV_NO_THIS_ID)
    {
        return NV_OK;
    }
    return ret;
}

u32 nv_read_rand_test(u32 nvid)
{
    u32 ret;
    u8* tempdata;
    u32 i= 0;
    struct nv_ref_data_info_stru ref_info = {0};
    struct nv_file_list_info_stru file_info = {0};

    ret = nv_search_byid(nvid, (u8*)NV_GLOBAL_CTRL_INFO_ADDR,&ref_info,&file_info);
    if(NV_OK != ret)
    {
        return ret;
    }
    if(ref_info.nv_len == 0)
    {
        return NV_ERROR;
    }

    tempdata = (u8*)nv_malloc((u32)(ref_info.nv_len) +1);
    if(NULL == tempdata)
    {
        return BSP_ERR_NV_MALLOC_FAIL;
    }

    ret = bsp_nvm_read(nvid,tempdata,ref_info.nv_len);
    if(NV_OK != ret)
    {
        nv_free(tempdata);
        return BSP_ERR_NV_READ_DATA_FAIL;
    }

    for(i=0;i<ref_info.nv_len;i++)
    {
        if((i%16) == 0)
        {
            printf("\n");
        }
        printf("%2.2x ",(u8)(*(tempdata+i)));
    }

    printf("\n");
    nv_free(tempdata);
    return NV_OK;
}

u32 nv_read_test_01(void)
{
    u32 itemid = 0xffff;
    u32 ret;

    ret = nv_read_rand_test(itemid);

    if(ret == BSP_ERR_NV_NO_THIS_ID)
    {
        return NV_OK;
    }

    return ret;
}

u32 nv_read_test_02(void)
{
    u32 itemid = 0xd007;
    u32 ret = NV_ERROR;

    u32 datalen  = 5;
    u32 off = 0;

    ret = nv_read_part_test(itemid,off,datalen);
    if(ret == BSP_ERR_NV_READ_DATA_FAIL)
    {
        return NV_OK;
    }

    return NV_ERROR;
}

u32 nv_read_test_03(void)
{
    u32 itemid = 0xd007;
    u32 ret = NV_ERROR;

    u32 datalen  = 1;
    u32 off = 4;

    ret = nv_read_part_test(itemid,off,datalen);
    if(ret == BSP_ERR_NV_READ_DATA_FAIL)
    {
        return NV_OK;
    }

    return NV_ERROR;
}

u32 nv_read_test_04(void)
{
    u32 itemid = 0xd007;
    u32 ret = NV_ERROR;

    u32 datalen  = 4;
    u32 off = 0;

    ret = nv_read_part_test(itemid,off,datalen);

    return ret;
}

u32 nv_read_test_05(void)
{
    u32 itemid = 0xd007;
    u32 ret = NV_ERROR;

    ret = nv_read_rand_test(itemid);

    return ret;
}

u32 nv_read_test_06(void)
{
    u32 itemid = 0xd007;
    u32 ret = NV_ERROR;
    u8* pdata = NULL;

    ret = bsp_nvm_read(itemid,pdata,sizeof(u32));
    if(ret == BSP_ERR_NV_INVALID_PARAM)
    {
        return NV_OK;
    }
    return ret;
}

u32 nv_read_test_07(void)
{
    u32 itemid = 0xd007;
    u32 ret = NV_ERROR;
    u32 pdata = 0;

    ret = bsp_nvm_read(itemid,(u8*)&pdata,1);
    if(ret == BSP_ERR_NV_ITEM_LEN_ERR)
    {
        return NV_OK;
    }
    return ret;
}

u32 nv_write_test01(void)
{
    u32 ret = NV_OK;
    u32 i = 0;
    u32 read_data = 0;
    u32 write_data = 0;

    for(i = 0;i<100;i++)
    {
         printf("\n****************第%d次读写开始**************************\n",i);
         write_data = i+60;
         ret = bsp_nvm_write(0xd007,(u8*)&write_data,sizeof(u32));
         if(NV_OK != ret)
         {
             printf("****write error*****i = %d\n",i);
             return ret;
         }
         ret = bsp_nvm_read(0xd007,(u8*)&read_data,sizeof(u32));
         if(ret)
         {
             printf("****read error*****i = %d\n",i);
             return ret;
         }
         if(read_data != write_data)
         {
            return NV_ERROR;
         }
    }
    return NV_OK;
}

u32 nv_write_test02(void)
{
    u32 ret = NV_OK;
    u32 i = 0;
    u32 read_data = 0;
    u32 write_data = 0;

    for(i = 0;i<100;i++)
    {
         printf("\n****************第%d次读写开始**************************\n",i);
         write_data = i+60;
         ret = bsp_nvm_write(0xd007,(u8*)&write_data,sizeof(u32));
         if(NV_OK != ret)
         {
             printf("****write error*****i = %d\n",i);
             return ret;
         }
         ret = bsp_nvm_read(0xd007,(u8*)&read_data,sizeof(u32));
         if(ret)
         {
             printf("****read error*****i = %d\n",i);
             return ret;
         }
    }
    return NV_OK;
}

u32 nv_write_test03(void)
{
    u32 ret = NV_ERROR;
    u32 itemid = 0xd007;

    u8* pdata = NULL;

    ret = bsp_nvm_write(itemid,pdata,sizeof(u32));
    if(ret == BSP_ERR_NV_INVALID_PARAM)
    {
        return NV_OK;
    }
    return ret;
}

u32 nv_write_test04(void)
{
    u32 ret = NV_ERROR;
    u32 itemid = 0xffff;

    u32 datalen  = 0;

    ret = bsp_nvm_write(itemid,(u8*)&datalen,sizeof(u32));
    if(ret == BSP_ERR_NV_NO_THIS_ID)
    {
        return NV_OK;
    }
    return ret;
}

u32 nv_write_test05(void)
{
    u32 ret = NV_ERROR;
    u32 itemid = 0xd007;
    u32 data = 56;

    ret = bsp_nvm_write(itemid,(u8*)&data,sizeof(u16));
    if(ret == BSP_ERR_NV_ITEM_LEN_ERR)
    {
        return NV_OK;
    }
    return ret;
}

u32 nv_write_test06(void);
u32 nv_write_test06(void)
{
    u32 ret = NV_ERROR;
    u32 itemid = 0xd007;
    u32 data = 56;

    ret = bsp_nvm_write(itemid,(u8*)&data,sizeof(u32));

    return ret;

}

u32 nv_backup_test(void)
{
    u32 ret = NV_ERROR;

    ret = bsp_nvm_backup();

    return ret;
}

u32 nv_revert_test(void)
{
    u32 ret = NV_ERROR;

    ret = bsp_nvm_backup();

    if(ret)
    {
        printf("backup failed!\n");
        return ret;
    }

    ret = bsp_nvm_revert();
    return ret;
}

u32 nv_revert_manuf_version_test(void)
{
    u32 ret = NV_ERROR;

    ret = bsp_nvm_update_default();
    if(ret)
    {
        printf("update default failed !\n");
        return ret;
    }
    ret = bsp_nvm_revert_default();

    return ret;
}
u32 nv_read_write_test(void);
u32 nv_read_write_test(void)
{
    u32 i;
    u32 ret;
    struct nv_ctrl_file_info_stru* ctrl_info = (struct nv_ctrl_file_info_stru*)NV_GLOBAL_CTRL_INFO_ADDR;
    struct nv_ref_data_info_stru* ref_info   = (struct nv_ref_data_info_stru*)(NV_GLOBAL_CTRL_INFO_ADDR+NV_GLOBAL_CTRL_INFO_SIZE\
        +NV_GLOBAL_FILE_ELEMENT_SIZE*ctrl_info->file_num);
    u8* pdata;

    pdata = (u8*)nv_malloc(3072);
    if(NULL == pdata)
    {
        return NV_ERROR;
    }
    for(i = 0;i<ctrl_info->ref_count;i++)
    {
        printf("*****************read & write 0x%x****************\n",ref_info->itemid);
        ret = bsp_nvm_read(ref_info->itemid,pdata,ref_info->nv_len);
        if(ret)
        {
            nv_free(pdata);
            return ret;
        }
        ret = bsp_nvm_write(ref_info->itemid,pdata,ref_info->nv_len);
        if(ret)
        {
            nv_free(pdata);
            return ret;
        }
        ref_info++;
    }
    nv_free(pdata);
    return NV_OK;
}
u32 nv_function_test(void);
u32 nv_function_test(void)
{
    u32 i;
    u32 ret;
    struct nv_ctrl_file_info_stru* ctrl_info = (struct nv_ctrl_file_info_stru*)NV_GLOBAL_CTRL_INFO_ADDR;
    struct nv_ref_data_info_stru* ref_info   = (struct nv_ref_data_info_stru*)(NV_GLOBAL_CTRL_INFO_ADDR+NV_GLOBAL_CTRL_INFO_SIZE\
        +NV_GLOBAL_FILE_ELEMENT_SIZE*ctrl_info->file_num);
    u8* pdata;
    u32 start,end;


    start = bsp_get_slice_value();

    pdata = (u8*)nv_malloc(3072);
    if(NULL == pdata)
    {
        return NV_ERROR;
    }
    for(i = 0;i<100;i++)
    {
        printf("*****************read & write 0x%x****************\n",ref_info->itemid);
        ret = bsp_nvm_read(ref_info->itemid,pdata,ref_info->nv_len);
        if(ret)
        {
            nv_free(pdata);
            return ret;
        }
        ret = bsp_nvm_write(ref_info->itemid,pdata,ref_info->nv_len);
        if(ret)
        {
            nv_free(pdata);
            return ret;
        }
        ref_info++;
    }
    end = bsp_get_slice_value();
    nv_free(pdata);

    printf("[%s]:slice 0x%x\n",__func__,end-start);
    return NV_OK;
}





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
