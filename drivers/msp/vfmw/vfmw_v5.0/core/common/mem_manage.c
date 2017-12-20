/******************************************************************************

  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

******************************************************************************
    �� �� ��   : mem_manage.c
    �� �� ��   : ����
    ��	  ��   :
    ��������   :
    ����޸�   :
    ��������   : firmware�Ĵ洢����ģ�飬����
		 1. �洢�ռ������ַ/�����ַ֮���ӳ��
		 2. ͨ�ô洢���ʺ���������Ĵ�����д��
		 3. �洢�ռ�ķ��������٣����ζ��ֲ���ϵͳ���죩
		 4. �Ĵ�����ַӳ�䣨���ζ��ֲ���ϵͳ���죩


    �޸���ʷ   :
    1.��    �� : 2009-01-07
    ��	  ��   : z56361
    �޸�����   :

******************************************************************************/

#include "mem_manage.h"
#include "sysconfig.h"
#include "vfmw_osal_ext.h"
#include "vfmw_log.h"


extern Vfmw_Osal_Func_Ptr vfmw_Osal_Func_Ptr_S;

#define	   MAX_MEM_MAN_RECORD_NUM    (MAX_CHAN_NUM*32)	//MAX_CHAN_NUM*5 ÿ��ͨ��ΪADD 11 ��
static UADDR  s_MemBaseAddr;
static UINT32  s_MemSize;
static UINT32  s_MemOffset;
static MEM_RECORD_S  s_MemRecord[MAX_MEM_MAN_RECORD_NUM];
MEM_DESC_S    (*mem_malloc)(UINT8 *MemName, UINT32 len, UINT32 align);
VOID	      (*mem_free)( MEM_DESC_S mem);

//OSAL_IRQ_SPIN_LOCK g_RecordLock;// = {0};

/************************************************************************/
/*    ��ʼ��								*/
/************************************************************************/
VOID MEM_InitMemManager(MEM_INIT_S *pMemInit)
{
    //OSAL_SpinLockIRQ(&g_RecordLock);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLock(G_RECORDLOCK);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet( &s_MemRecord, 0, sizeof(MEM_RECORD_S)*MAX_MEM_MAN_RECORD_NUM );

    s_MemBaseAddr     = pMemInit->MemBaseAddr;
    s_MemSize	      = pMemInit->MemSize;
    s_MemOffset	      = 0;

    //OSAL_SpinUnLockIRQ(&g_RecordLock);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinUnlock(G_RECORDLOCK);

    return;
}

/************************************************************************/
/*    ָ���ڴ����							*/
/************************************************************************/
VOID MEM_ManagerWithOperation(VDEC_OPERATION_S *pArgs)
{
    mem_malloc = UINT64_PTR(pArgs->mem_malloc);
    mem_free = UINT64_PTR(pArgs->mem_free);

    return;
}
/************************************************************************/
/*    ���ָ���ڴ����							*/
/************************************************************************/
VOID MEM_ManagerClearOperation(VOID)
{
    mem_malloc = NULL;
    mem_free = NULL;

    return;
}
/************************************************************************/
/*    ��Ӽ�¼								*/
/************************************************************************/
SINT32 MEM_AddMemRecord(UADDR PhyAddr, VOID* VirAddr, UINT32 Length)
{
    SINT32  i;

    if ((PhyAddr == 0) || (VirAddr == NULL) || (Length == 0))
    {
	return MEM_MAN_OK;
    }

    //OSAL_SpinLockIRQ(&g_RecordLock);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLock(G_RECORDLOCK);

    for ( i = 0; i < MAX_MEM_MAN_RECORD_NUM; i++ )
    {
	if ( s_MemRecord[i].Length == 0 )
	{
	    break;
	}
    }

    if ( i < MAX_MEM_MAN_RECORD_NUM )
    {
	s_MemRecord[i].PhyAddr = PhyAddr;
	s_MemRecord[i].VirAddr = VirAddr;
	s_MemRecord[i].Length  = Length;

	//OSAL_SpinUnLockIRQ(&g_RecordLock);
	vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinUnlock(G_RECORDLOCK);

	return MEM_MAN_OK;
    }
    else
    {
	//OSAL_SpinUnLockIRQ(&g_RecordLock);
	vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinUnlock(G_RECORDLOCK);

	return MEM_MAN_ERR;
    }
}

/************************************************************************/
/*    ɾ����¼								*/
/************************************************************************/
SINT32 MEM_DelMemRecord(UADDR PhyAddr, VOID* VirAddr, UINT32 Length)
{
    SINT32  i;

    if ((PhyAddr == 0) || (VirAddr == NULL) || (Length == 0))
    {
	return MEM_MAN_OK;
    }

    //OSAL_SpinLockIRQ(&g_RecordLock);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLock(G_RECORDLOCK);

    for ( i = 0; i < MAX_MEM_MAN_RECORD_NUM; i++ )
    {
	if ( 0 == s_MemRecord[i].Length )
	{
	    continue;
	}

	if ( PhyAddr == s_MemRecord[i].PhyAddr && VirAddr == s_MemRecord[i].VirAddr
	     && Length == s_MemRecord[i].Length )
	{
	    s_MemRecord[i].Length  = 0;
	    s_MemRecord[i].PhyAddr = 0;
	    s_MemRecord[i].VirAddr = 0;

	    //OSAL_SpinUnLockIRQ(&g_RecordLock);
	    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinUnlock(G_RECORDLOCK);

	    return MEM_MAN_OK;
	}
    }

    //OSAL_SpinUnLockIRQ(&g_RecordLock);
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinUnlock(G_RECORDLOCK);

    return MEM_MAN_ERR;
}
/************************************************************************/
/*   ��ַת���� �����ַ -> �����ַ					*/
/************************************************************************/
VOID *MEM_Phy2Vir(UADDR PhyAddr)
{
    UINT32  i;
    UINT8   *VirAddr;

    VirAddr = (VOID *)0;

    for ( i = 0; i < MAX_MEM_MAN_RECORD_NUM; i++ )
    {
	if ( s_MemRecord[i].Length == 0)
	{
	    continue;
	}

	if ( (PhyAddr >= s_MemRecord[i].PhyAddr) && (PhyAddr < s_MemRecord[i].PhyAddr + s_MemRecord[i].Length) )
	{
	    VirAddr = s_MemRecord[i].VirAddr + (PhyAddr - s_MemRecord[i].PhyAddr);
	    break;
	}
    }

    return (VOID *)VirAddr;
}


/************************************************************************/
/*   ��ַת���� �����ַ -> �����ַ					*/
/************************************************************************/
UADDR MEM_Vir2Phy(UINT8 *VirAddr)
{
    UINT32  i;

    UADDR  PhyAddr = 0;

    for ( i = 0; i < MAX_MEM_MAN_RECORD_NUM; i++ )
    {
	if ( s_MemRecord[i].Length == 0)
	{
	    continue;
	}

	if ( (VirAddr >= s_MemRecord[i].VirAddr) && (VirAddr < s_MemRecord[i].VirAddr + s_MemRecord[i].Length) )
	{
	    PhyAddr = s_MemRecord[i].PhyAddr + (VirAddr - s_MemRecord[i].VirAddr);
	    break;
	}
    }

    return PhyAddr;
}


/************************************************************************/
/*   mem������ ��ָ���������ַд��һ��word				*/
/************************************************************************/
VOID MEM_WritePhyWord(UADDR PhyAddr, UINT32 Data32)
{
    UINT32 *pDst;

    pDst = (UINT32 *)MEM_Phy2Vir(PhyAddr);
    if (pDst != NULL)
    {
	*pDst = Data32;
    }
}

UINT32 MEM_ReadPhyWord(UADDR PhyAddr)
{
    UINT32 *pDst;

    pDst = (UINT32 *)MEM_Phy2Vir(PhyAddr);
    if (pDst == NULL)
    {
	return 0;
    }

    return *(volatile UINT32 *)pDst;
}

/************************************************************************/
/*   mem������ ����һƬ���ݣ�phy -> phy��				*/
/************************************************************************/
VOID MEM_CopyPhy2Phy(UADDR DstPhyAddr, UADDR SrcPhyAddr, UINT32 Length)
{
    UINT8   *pDst, *pSrc;

    pDst = (UINT8*)MEM_Phy2Vir( DstPhyAddr );
    pSrc = (UINT8*)MEM_Phy2Vir( SrcPhyAddr );

    if ( NULL != pDst && NULL != pSrc )
    {
	//memcpy( pDst, pSrc, Length );
	vfmw_Osal_Func_Ptr_S.pfun_Osal_MemCpy( pDst, pSrc, Length );
    }
}

/************************************************************************/
/*   mem������ ����һƬ���ݣ�phy -> vir��				*/
/************************************************************************/
VOID MEM_CopyPhy2Vir( UINT8 *DstVirAddr, UADDR SrcPhyAddr, UINT32 Length )
{
    UINT8   *pSrc;

    pSrc = (UINT8*)MEM_Phy2Vir( SrcPhyAddr );

    if ( NULL != DstVirAddr && NULL != pSrc )
    {
	//memcpy( DstVirAddr, pSrc, Length );
	vfmw_Osal_Func_Ptr_S.pfun_Osal_MemCpy( DstVirAddr, pSrc, Length );
    }
}

/************************************************************************/
/*   mem������ ����һƬ���ݣ�vir -> phy��				*/
/************************************************************************/
VOID MEM_CopyVir2Phy(UADDR DstPhyAddr, UINT8* SrcVirAddr, UINT32 Length)
{
    UINT8   *pDst;

    pDst = (UINT8*)MEM_Phy2Vir( DstPhyAddr );

    if ( NULL != pDst && NULL != SrcVirAddr )
    {
	//memcpy( pDst, SrcVirAddr, Length );
	vfmw_Osal_Func_Ptr_S.pfun_Osal_MemCpy( pDst, SrcVirAddr, Length );
    }
}


/************************************************************************/
/*   �����ڴ��								*/
/************************************************************************/
SINT32	MEM_AllocMemBlock(CHAR *MemName, UADDR ExpectedPhyAddr, UINT32 ExpectedLen, MEM_RECORD_S *pMemRecord, SINT32 IsCached)
{
    UINT8 *ptr = NULL;
    UADDR PhyAddr;
    UADDR MmuAddr;
    MEM_DESC_S AllocMem;
    MEM_MODE_E MmuMode;

    // ��ڲ������
    if ( (NULL == pMemRecord) || (0 == ExpectedLen) )
    {
	return MEM_MAN_ERR;
    }

    MmuMode = pMemRecord->eMode;

    // ��������Ϣ�ṹ
    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet( pMemRecord, 0, sizeof(MEM_RECORD_S) );
    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet( &AllocMem,  0, sizeof(MEM_DESC_S) );

    AllocMem.eMode = MmuMode;

    // �������ӳ���ַ�ռ�: �����ַ --> �����ַ */
    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelMemAlloc(MemName, ExpectedLen, 4, IsCached, &AllocMem);

    if ( NULL == AllocMem.VirAddr )
    {
	return MEM_MAN_ERR;  /* �쳣���� */
    }
    else
    {
	PhyAddr = AllocMem.PhyAddr;
	MmuAddr = AllocMem.MmuAddr;
	ptr = UINT64_PTR(AllocMem.VirAddr);
	ExpectedLen  = AllocMem.Length;

	switch (AllocMem.eMode)
	{
	    case MEM_CMA_CMA:
	    {
		ExpectedPhyAddr = PhyAddr;
		if ( (0 == PhyAddr) || (NULL == ptr) )
		{
		    return MEM_MAN_ERR;
		}
		break;
	    }

	    case MEM_CMA_MMU:
	    case MEM_MMU_CMA:
	    {
		ExpectedPhyAddr = MmuAddr;
		if ( (0 == PhyAddr) ||(0 == MmuAddr) || (NULL == ptr) )
		{
		    return MEM_MAN_ERR;
		}
		break;
	    }

	    case MEM_MMU_MMU:
	    {
		ExpectedPhyAddr = MmuAddr;
		if ( (0 == MmuAddr) || (NULL == ptr) )
		{
		    return MEM_MAN_ERR;
		}
		break;
	    }

	    default:
	    {
		return MEM_MAN_ERR;
	    }
	}
    }

    pMemRecord->PhyAddr = ExpectedPhyAddr;
    pMemRecord->VirAddr = ptr;
    pMemRecord->Length	= ExpectedLen;
    s_MemOffset += ExpectedLen;
    s_MemOffset = (s_MemOffset + 3) & (~3);

    return MEM_MAN_OK;
}


/************************************************************************/
/*   ӳ��Ĵ���								*/
/************************************************************************/
SINT32	MEM_MapRegisterAddr(UADDR RegStartPhyAddr, UINT32 RegByteLen, MEM_RECORD_S *pMemRecord)
{
    UINT8 *ptr;

    // ��ڲ������
    if ( (NULL == pMemRecord) || (0 == RegStartPhyAddr) )
    {
	return MEM_MAN_ERR;
    }

    // ��������Ϣ�ṹ
    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet( pMemRecord, 0, sizeof(MEM_RECORD_S) );

    // ��ַӳ��: IO_ADDRESS */
    ptr = (UINT8*)vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelRegisterMap(RegStartPhyAddr);

    if ( NULL != ptr )
    {
	pMemRecord->PhyAddr = RegStartPhyAddr;
	pMemRecord->VirAddr = ptr;
	pMemRecord->Length = RegByteLen;
	return MEM_MAN_OK;
    }

    return MEM_MAN_ERR;
}

/************************************************************************/
/*   �Ĵ���ȥӳ��							*/
/************************************************************************/
VOID MEM_UnmapRegisterAddr(UADDR PhyAddr, UINT8 *VirAddr)
{
    // ��ڲ������
    if ( (0 == PhyAddr) || (0 == VirAddr) )
    {
	return;
    }

    // ��ַȥӳ��: IO_ADDRESS */
    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelRegisterUnMap(VirAddr);

    return;
}

/************************************************************************/
/*   �ͷ��ڴ��								*/
/************************************************************************/
VOID MEM_ReleaseMemBlock(UADDR PhyAddr, UINT8 *VirAddr )
{
    MEM_DESC_S FreeMem;
    UINT32 i;

    // ��ڲ������
    if ( (0 == PhyAddr) || (0 == VirAddr) )
    {
	return;
    }

    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet(&FreeMem, 0, sizeof(MEM_DESC_S));

    FreeMem.PhyAddr = PhyAddr;
    FreeMem.MmuAddr = PhyAddr;
    FreeMem.VirAddr = PTR_UINT64(VirAddr);

    for ( i = 0; i < MAX_MEM_MAN_RECORD_NUM; i++ )
    {
	if ( s_MemRecord[i].Length == 0)
	{
	    continue;
	}

	if ( (PhyAddr >= s_MemRecord[i].PhyAddr) && (PhyAddr < s_MemRecord[i].PhyAddr + s_MemRecord[i].Length) )
	{
	    FreeMem.Length = s_MemRecord[i].Length;
	    break;
	}
    }

    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelMemFree(&FreeMem);

    /*
	ɾ��һ���ڴ�����Ӧͬ��ɾ��s_MemRecord�ж�Ӧ�ļ�¼������˵�ַ����������������á�
	����firmware�У��ڴ��������ͷ�ֻ�ڽ������Ĵ��������ٲ����вŻ���У����Ժ����������⡣
    */

    return;
}

/************************************************************************/
/*   ��ַӳ�䣺 �����ַ -> �����ַ					*/
/************************************************************************/
VOID MEM_NsMapToSec(UADDR PhyAddr, UINT8 **VirAddr,UINT32 Length)
{
#ifdef ENV_SOS_KERNEL
    *VirAddr = vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelNsMmap(PhyAddr, Length);
#endif
    return;
}

/************************************************************************/
/*   ��ַȥӳ��								*/
/************************************************************************/
VOID MEM_NsInSecUnMap(UINT8* VirAddr)
{
#ifdef ENV_SOS_KERNEL
    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelNsMunMap(VirAddr);
#endif
    return;
}
