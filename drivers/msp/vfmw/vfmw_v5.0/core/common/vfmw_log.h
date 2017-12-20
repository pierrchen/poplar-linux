#ifndef __VFMW_LOG_H__
#define __VFMW_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "vfmw.h"

/* debug message print control */
#define	 DEFAULT_PRINT_ENABLE	(0x3)
#define	 DEFAULT_PRINT_DEVICE	(DEV_SCREEN)   // Ĭ�� 'DEV_SCREEN'������ӡ����Ļ

#define	 LOG_OK	      (0)
#define	 LOG_ERR     (-1)


/* type define */
typedef enum
{
    DEV_SCREEN = 1,
    DEV_SYSLOG,
    DEV_FILE,
    DEV_MEM
} PRINT_DEVICE_TYPE;

typedef enum
{
    PRN_FATAL = 0,	 // 0.	�����쳣��fatal error��������δ֪�쳣��VDM��������Ϣ
    PRN_ERROR,		 // 1.	һ���쳣��error���������﷨����
    PRN_CTRL,		 // 2.	������Ϣ, ����״̬�ƣ��ڲ�״̬����ת��Ϣ��
    PRN_VDMREG,		 // 3.	VDM�Ĵ������������ƼĴ�����״̬�Ĵ���

    PRN_DNMSG,		 // 4.	����������Ϣ������Ϣ�ֵ�ֵ��ӡ
    PRN_RPMSG,		 // 5.	�޲���Ϣ���޲���Ϣ������Ϣ��
    PRN_UPMSG,		 // 6.	������Ϣ��������Ϣ������Ϣ��
    PRN_STREAM,		 // 7.	������Ϣ����ÿ������������Ϣ����ַ�����ȵȣ�

    PRN_STR_HEAD,	 // 8.	����ͷ�ֽڣ���ǰ8�ֽڣ����ڲ��ұȶԣ�
    PRN_STR_TAIL,	 // 9.	����β�ֽڣ�����8�ֽڣ����ڲ��ұȶԣ�
    PRN_STR_BODY,	 // 10. �����в��ֽڣ���ǰ��8�ֽ�֮���������
    PRN_IMAGE,		 // 11. ����ͼ����Ϣ��ͼ���id����ַ�����Եȣ�

    PRN_QUEUE,		 // 12. ����ͼ������Ϣ�����е�ͼ�������λ�õȣ�
    PRN_REF,		 // 13. �ο�ͼ��Ϣ��H264��ӡLIST�����Ϣ��MPEG��ӡ�ο�ͼ����ά������Ϣ��
    PRN_DPB,		 // 14. DPB����Ϣ����dpb��صĸ�����Ϣ��
    PRN_POC,		 // 15. POC����Ϣ��POC�Ľ���ͼ��㣩

    PRN_MARK_MMCO,	 // 16. MARK & MMCO������Ϣ��MARK��MMCO��������Ϣ���ɱ���ִ�еĹ��̣�
    PRN_SEQ,		 // 17. ���м���Ҫ�﷨��Ϣ��H264��ӡSPS��MPEG2��ӡsequence��Ϣ��MPEG4��ӡVOL�����ϲ���Ϣ
    PRN_PIC,		 // 18. ͼ����Ҫ�﷨��Ϣ��H264��ӡPPS��MPEG2��ӡpicture��MPEG4��ӡVOP/SHVOPͷ
    PRN_SLICE,		 // 19. slice����Ҫ�﷨��Ϣ����H264������

    PRN_SEI,		 // 20. SEI��Ϣ����H264������
    PRN_SE,		 // 21. H264�﷨��Ϣ,��H264������
    PRN_DBG,		 // 22. ������Ϣ
    PRN_BLOCK,		 // 23. �����߳�������Ϣ

    PRN_SCD_REGMSG,	 // 24.
    PRN_SCD_STREAM,	 // 25.
    PRN_SCD_INFO,	 // 26. ��ӡSCDһЩ����״̬����Ϣ
    PRN_INT_TIME,	 // 27. VDH�ж���Ӧʱ��

    PRN_TEE,		 // 28. ��ӡ��ȫ���������Ϣ
    PRN_PTS,		 // 29. ��ӡ��PTS������ص���Ϣ
    PRN_DEC_MODE,	 // 30. ����ģʽ�л���Ϣ
    PRN_FS,		 // 31. ��̬֡���ӡ
    PRN_ALWS = 32	 // 32. ���ܿش�ӡ
} PRINT_MSG_TYPE;

/* state type */
typedef enum
{
    STATE_RCV_RAW = 0,		 //0. ����RAW����
    STATE_SCD_START,		 //1. ����SCD
    STATE_SCD_INTERRUPT,	 //2. SCD�ж�

    STATE_DECSYNTAX_SEG,	 //3. �﷨����SEG����
    STATE_GENERATE_DECPARAM,	 //4. ���ɽ������

    STATE_VDH_START,		 //5. ����VDH
    STATE_VDH_INTERRUPT,	 //6. VDH�ж�
    STATE_VDH_REPAIR,		 //7. VDH�޲�
    STATE_1D_TO_QUEUE,		 //8. 1Dͼ�����������

    STATE_VO_RCV_IMG,		 //12. VO��ȡͼ��
    STATE_VO_REL_IMG		 //13. VO�ͷ�ͼ��
} VFMW_STATE_TYPE_E;

typedef enum
{
    VFMW_STRING_VDH_IRQ = 0,
    VFMW_STRING_VDH_IRQ_SAFE,
    VFMW_STRING_VDH1_IRQ,
    VFMW_STRING_VDH2_IRQ,
    VFMW_STRING_SCD_IRQ,
    VFMW_STRING_SCD_IRQ_SAFE,
    VFMW_STRING_SCD1_IRQ,
    VFMW_STRING_CHAN_CTX,
    VFMW_STRING_CHAN_VDH,
    VFMW_STRING_CHAN_SCD,
    VFMW_STRING_DSPCTXMEM,
    VFMW_STRING_DNR_IRQ,
    VFMW_STRING_BTL_IRQ,
    VFMW_STRING_VDH_MMU_IRQ,
    VFMW_STRING_VDH_MMU_IRQ_SAFE,
    VFMW_STRING_VDH_HAL_MEM,
    VFMW_STRING_VP9_PROB_MEM,
    VFMW_STRING_SCD_MSG_MEM,
    VFMW_STRING_RAW_SAVE_PATH,
    VFMW_STRING_SEG_SAVE_PATH,
    VFMW_STRING_YUV_SAVE_PATH,
    VFMW_STRING_YUV_2D_SAVE_NAME,
    VFMW_STRING_YUV_1D_SAVE_NAME,
    VFMW_STRING_META_SAVE_NAME,
    VFMW_STRING_META_REORDER_SAVE_NAME,
    VFMW_STRING_HDR_EL_SAVE_NAME,
    VFMW_STRING_TRACER,
    VFMW_STRING_BIGTILE_ID_YUV,
    VFMW_STRING_BUTT
}VFMW_STRING_E;

#ifdef ENV_SOS_KERNEL
#define VDH_IRQ_NAME_INDEX     (VFMW_STRING_VDH_IRQ_SAFE)
#define SCD_IRQ_NAME_INDEX     (VFMW_STRING_SCD_IRQ_SAFE)
#define MMU_IRQ_NAME_INDEX     (VFMW_STRING_VDH_MMU_IRQ_SAFE)
#else
#define VDH_IRQ_NAME_INDEX     (VFMW_STRING_VDH_IRQ)
#define SCD_IRQ_NAME_INDEX     (VFMW_STRING_SCD_IRQ)
#define MMU_IRQ_NAME_INDEX     (VFMW_STRING_VDH_MMU_IRQ)
#endif

/* extern value */
extern	UINT32	 g_PrintEnable;
extern	UINT32	 g_PrintDevice;
extern	SINT32	 g_TraceCtrl;
extern	SINT32	 g_TraceBsPeriod;
extern	SINT32	 g_TraceFramePeriod;
extern	SINT32	 g_TraceImgPeriod;


/* assert define */
#define VFMW_ASSERT(Condition)						 \
do									 \
{									 \
    if (!(Condition))							 \
    {									 \
	dprint(PRN_ERROR, "[%s %d]assert warning\n",__func__,__LINE__);	 \
	return;								 \
    }									 \
}while(0)

#define VFMW_ASSERT_RETURN(Condition, Ret)				 \
do									 \
{									 \
    if (!(Condition))							 \
    {									 \
	dprint(PRN_ERROR, "[%s %d]assert warning\n",__func__,__LINE__);	 \
	return Ret;							 \
    }									 \
}while(0)

#define CHECK_NULL_PTR_ReturnValue(parPoint,ReturnValue)		 \
do									 \
{									 \
    if (NULL == parPoint)						 \
    {									 \
	dprint(PRN_FATAL, "NULL pointer: %s, L%d\n", __FILE__, __LINE__);\
	return ReturnValue;						 \
    }									 \
}while(0)

#define CHECK_NULL_PTR_Return(parPoint)					 \
do									 \
{									 \
    if (NULL == parPoint)						 \
    {									 \
	dprint(PRN_FATAL, "NULL pointer: %s, L%d\n", __FILE__, __LINE__);\
	return;								 \
    }									 \
}while(0)


/* macro define */
#ifndef HI_ADVCA_FUNCTION_RELEASE

#ifndef ENV_SOS_KERNEL
#define dprint(type, fmt, arg...)  LOG_Print(type, fmt, ##arg)

#else
#define dprint(type, fmt, arg...)					 \
do{									 \
    if ((((PRN_ALWS == type) || (0 != (g_PrintEnable & (1 << type))))	 \
       && (DEV_SCREEN == g_PrintDevice)))				 \
    {	OSAL_FP_print("S: [%d] ", __LINE__);				 \
	OSAL_FP_print(fmt, ##arg);					 \
    }									 \
}while(0)
#endif

#else

#define dprint(type, fmt, arg...) LOG_PrintNothing()
#endif

#define pos()  dprint(PRN_ALWS,"%s: L%d\n", __FUNCTION__, __LINE__)

#define dprint_dec(v)	   dprint(PRN_ALWS, "%s = %d\n",   #v, (v))
#define dprint_hex(v)	   dprint(PRN_ALWS, "%s = 0x%x\n", #v, (v))
#define dprint_str(v)	   dprint(PRN_ALWS, "%s = %s\n",   #v, (v))

#define LOG_FUNC_ENTRY()   dprint(PRN_ALWS, "[%s %d] entry!\n",__func__,__LINE__)
#define LOG_FUNC_EXIT()	   dprint(PRN_ALWS, "[%s %d] exit! \n",__func__,__LINE__)
#define LOG_NOT_SUPPORT()  dprint(PRN_ALWS, "[%s %d] not support now!\n",__func__,__LINE__)

#define REC_POS(Data)	   LOG_AddTrace((SINT8*)__FUNCTION__, __LINE__, (SINT32)Data)

/* function */
SINT32 LOG_IsPrintEnable(PRINT_MSG_TYPE Type);
VOID   LOG_PrintNothing(VOID);
SINT32 LOG_IsRecPosEnable(VFMW_STATE_TYPE_E Type);
VOID   LOG_RecPos(SINT32 SmID, VFMW_STATE_TYPE_E VfmwStateType, UINT32 *LastRecPosTimeInMs, SINT8 *FunctionName, SINT32 LineNumber);
CHAR  *LOG_GetString(VFMW_STRING_E enStringName);

#ifndef ENV_SOS_KERNEL
SINT32 LOG_Print(UINT32 type, const CHAR *format, ...);
SINT32 LOG_CreateTracer(VOID);
VOID   LOG_DestroyTracer(VOID);
VOID   LOG_AddTrace( SINT8 *pFuncName, SINT32 nLine, SINT32 Data );
VOID   LOG_PrintTracer( SINT32 TraceDepth );
#endif

#ifdef __cplusplus
}
#endif

#endif
