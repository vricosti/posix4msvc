#ifndef _WIN_DEVBLK_H_
#define _WIN_DEVBLK_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winioctl.h>

#ifdef  __cplusplus
extern "C" {
#endif


	typedef struct _DISKINFO
	{
		DWORD			u32DeviceNumber;
		char			szSerialNumber[1000];
		char			szModelNumber[1000];
		char			szVendorId[1000];
		char			szProductRevision[1000];
		char			szDevicePath[1000];
		char			szShortDevicePath[MAX_PATH + 1];
		int				canBePartitioned;
		DISK_GEOMETRY_EX diskGeometry;
		DRIVE_LAYOUT_INFORMATION_EX driveLayoutInfo;

	} DISKINFO, *PDISKINFO;


	typedef struct _PARTITION_INFO
	{
		PDISKINFO pDiskInfo;
		DWORD PartitionStyle;
		LARGE_INTEGER StartingOffset;
		LARGE_INTEGER PartitionLength;
		DWORD PartitionNumber;
		BOOLEAN RewritePartition;
		union {
			PARTITION_INFORMATION_MBR Mbr;
			PARTITION_INFORMATION_GPT Gpt;
		} DUMMYUNIONNAME;

		// Volume info
		CHAR    szRootPathName[MAX_PATH + 1];
		CHAR	szVolumeName[MAX_PATH + 1];
		CHAR	szVolumePathName[MAX_PATH + 1];
		CHAR	szFileSystemName[MAX_PATH + 1];
		DWORD	dwSerialNumber;
		DWORD	dwFileSystemFlags;


	} PARTITION_INFO, *PPARTITION_INFO;



#ifdef  __cplusplus
}
#endif



#endif
