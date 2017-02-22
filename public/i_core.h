
#ifndef _PUBLIC_I_CORE_H
#define _PUBLIC_I_CORE_H

#include "macros.h"
#include "persistid.h"
#include "i_var.h"
#include "i_var_list.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
//��־����
enum LOG_TYPE
{
	LOG_RENDER,//��Ⱦ���
	LOG_GUI,//GUi��ؼ��
	LOG_NET,//������ؼ��
	LOG_CORE,//Core��ؼ��
	LOG_PHYSICS,//������
	LOG_LOGIC,//�߼����
	LOG_FILEIO,//�ļ�IO���
	LOG_FRAME,//��Ⱦ֡����
	LOG_PERF,//���ܳ�ʱ��ؼ��
	LOG_TYPE_MAX,//���������
};

//д��һ����־ (֧��1024���ֽ�������Ч��
extern bool fm_tracelog(const char * info, LOG_TYPE log_type = LOG_LOGIC);

//��ȡһ����־
extern char* fm_logread(LOG_TYPE log_type = LOG_LOGIC);

#ifdef __cplusplus
}
#endif // __cplusplus

// ���Ľӿ�

class IEntity;
class IInterface;
class IEntInfo;

class ICore
{
public:
	virtual ~ICore() = 0;
	// ��ģ���ڴ����
	virtual void* MemoryCheckAllocDebug(size_t size, const char* file,
		int line) = 0;
	virtual void* MemoryCheckAlloc(size_t size) = 0;
	virtual void MemoryCheckFree(void* ptr, size_t size) = 0;
	virtual void* MemoryAlloc(size_t size) = 0;
	virtual void MemoryFree(void* ptr) = 0;

	// ɾ��ʵ��
	virtual bool DeleteEntity(const PERSISTID& id) = 0;
	// �ͷŹ��ܽӿ�
	virtual void ReleaseInterface(IInterface* pInterface) = 0;
	// ���ʵ��
	virtual IEntity* GetEntity(const PERSISTID& id) = 0;
};

inline ICore::~ICore() {}

#endif