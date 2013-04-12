#ifndef __TIMEDRAM_H__
#define __TIMEDRAM_H__
#include "MemoryWrite.h"
#include "Device/Device.pkg"
#include "TimedBuffers/TimedBuffers.pkg"
#include <vector>

//##TODO## This class has been superseded by our TimedBuffer structure. It is only
//referenced now by our old VDP core, and it isn't safe to use across DLL boundaries. This
//class should be removed along with the old VDP core as soon as the new VDP core is
//complete.
class TimedRAM :public MemoryWrite
{
public:
	//Typedefs
	typedef RandomTimeAccessBuffer<unsigned char, double>::AccessTarget AccessTarget;
	typedef RandomTimeAccessBuffer<unsigned char, double>::Timeslice Timeslice;
	typedef RandomTimeAccessBuffer<unsigned char, double>::WriteInfo WriteInfo;

public:
	//Constructors
	TimedRAM(const std::wstring& aimplementationName, const std::wstring& ainstanceName, unsigned int amoduleID);
	virtual bool Construct(IHeirarchicalStorageNode& node);

	//Initialization functions
	virtual void Initialize();

	//Execute functions
	virtual void ExecuteRollback();
	virtual void ExecuteCommit();

	//Memory interface functions
	virtual IBusInterface::AccessResult ReadInterface(unsigned int interfaceNumber, unsigned int location, Data& data, IDeviceContext* caller, double accessTime, unsigned int accessContext);
	virtual IBusInterface::AccessResult WriteInterface(unsigned int interfaceNumber, unsigned int location, const Data& data, IDeviceContext* caller, double accessTime, unsigned int accessContext);
	virtual void TransparentReadInterface(unsigned int interfaceNumber, unsigned int location, Data& data, IDeviceContext* caller, unsigned int accessContext);
	virtual void TransparentWriteInterface(unsigned int interfaceNumber, unsigned int location, const Data& data, IDeviceContext* caller, unsigned int accessContext);

	//Buffer access functions
	void ReadCommitted(unsigned int location, Data& data);
	void ReadCommitted(unsigned int location, double writeTime, Data& data);
	//##TODO## Consider removing this function
	void GetLatestBufferCopy(std::vector<unsigned char>& buffer) const;
	void ReadBuffer(unsigned int location, Data& data, const AccessTarget& accessTarget);
	void WriteBuffer(unsigned int location, const Data& data, const AccessTarget& accessTarget);

	//Memory locking functions
	virtual bool IsMemoryLockingSupported() const;
	virtual void LockMemoryBlock(unsigned int location, unsigned int size, bool state);
	virtual bool IsByteLocked(unsigned int location) const;

	//Buffer time management functions
	Timeslice GetLatestTimeslice();
	void AdvancePastTimeslice(const Timeslice& targetTimeslice);
	void AdvanceToTimeslice(const Timeslice& targetTimeslice);
	void AdvanceByTime(double step, const Timeslice& targetTimeslice);
	bool AdvanceByStep(const Timeslice& targetTimeslice);
	double GetNextWriteTime(const Timeslice& targetTimeslice);
	WriteInfo GetWriteInfo(unsigned int index, const Timeslice& targetTimeslice);
	void AddTimeslice(double timeslice);

	//Savestate functions
	virtual void LoadState(IHeirarchicalStorageNode& node);
	virtual void SaveState(IHeirarchicalStorageNode& node) const;

private:
	RandomTimeAccessBuffer<unsigned char, double> memory;
	std::vector<bool> memoryLocked;
};

#endif
