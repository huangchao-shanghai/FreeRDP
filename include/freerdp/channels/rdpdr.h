/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * Device Redirection Virtual Channel
 *
 * Copyright 2010-2011 Vic Lee
 * Copyright 2010-2012 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FREERDP_CHANNEL_RDPDR_H
#define FREERDP_CHANNEL_RDPDR_H

#include <winpr/nt.h>
#include <winpr/crt.h>
#include <winpr/file.h>
#include <winpr/synch.h>
#include <winpr/thread.h>
#include <winpr/stream.h>
#include <winpr/interlocked.h>

#include <freerdp/utils/list.h>
#include <freerdp/utils/svc_plugin.h>

/* RDPDR_HEADER.Component */
enum RDPDR_CTYP
{
	RDPDR_CTYP_CORE = 0x4472,
	RDPDR_CTYP_PRN = 0x5052
};

/* RDPDR_HEADER.PacketId */
enum RDPDR_PAKID
{
	PAKID_CORE_SERVER_ANNOUNCE = 0x496E,
	PAKID_CORE_CLIENTID_CONFIRM = 0x4343,
	PAKID_CORE_CLIENT_NAME = 0x434E,
	PAKID_CORE_DEVICELIST_ANNOUNCE = 0x4441,
	PAKID_CORE_DEVICE_REPLY = 0x6472,
	PAKID_CORE_DEVICE_IOREQUEST = 0x4952,
	PAKID_CORE_DEVICE_IOCOMPLETION = 0x4943,
	PAKID_CORE_SERVER_CAPABILITY = 0x5350,
	PAKID_CORE_CLIENT_CAPABILITY = 0x4350,
	PAKID_CORE_DEVICELIST_REMOVE = 0x444D,
	PAKID_CORE_USER_LOGGEDON = 0x554C,
	PAKID_PRN_CACHE_DATA = 0x5043,
	PAKID_PRN_USING_XPS = 0x5543
};

/* CAPABILITY_HEADER.CapabilityType */
enum RDPDR_CAP_TYPE
{
	CAP_GENERAL_TYPE = 0x0001,
	CAP_PRINTER_TYPE = 0x0002,
	CAP_PORT_TYPE = 0x0003,
	CAP_DRIVE_TYPE = 0x0004,
	CAP_SMARTCARD_TYPE = 0x0005
};

/* CAPABILITY_HEADER.Version */
enum RDPDR_CAP_VERSION
{
	GENERAL_CAPABILITY_VERSION_01 = 0x00000001,
	GENERAL_CAPABILITY_VERSION_02 = 0x00000002,
	PRINT_CAPABILITY_VERSION_01 = 0x00000001,
	PORT_CAPABILITY_VERSION_01 = 0x00000001,
	DRIVE_CAPABILITY_VERSION_01 = 0x00000001,
	DRIVE_CAPABILITY_VERSION_02 = 0x00000002,
	SMARTCARD_CAPABILITY_VERSION_01 = 0x00000001
};

/* DR_DEVICE_IOREQUEST.MajorFunction */
enum IRP_MJ
{
	IRP_MJ_CREATE = 0x00000000,
	IRP_MJ_CLOSE = 0x00000002,
	IRP_MJ_READ = 0x00000003,
	IRP_MJ_WRITE = 0x00000004,
	IRP_MJ_DEVICE_CONTROL = 0x0000000E,
	IRP_MJ_QUERY_VOLUME_INFORMATION = 0x0000000A,
	IRP_MJ_SET_VOLUME_INFORMATION = 0x0000000B,
	IRP_MJ_QUERY_INFORMATION = 0x00000005,
	IRP_MJ_SET_INFORMATION = 0x00000006,
	IRP_MJ_DIRECTORY_CONTROL = 0x0000000C,
	IRP_MJ_LOCK_CONTROL = 0x00000011
};

/* DR_DEVICE_IOREQUEST.MinorFunction */
enum IRP_MN
{
	IRP_MN_QUERY_DIRECTORY = 0x00000001,
	IRP_MN_NOTIFY_CHANGE_DIRECTORY = 0x00000002
};

/* DR_CREATE_REQ.CreateDisposition */

/* DR_CREATE_REQ.CreateOptions [MS-SMB2] */

/* DR_CREATE_REQ.DesiredAccess [MS-SMB2] */

/* DR_CREATE_RSP.Information */
/* DR_DRIVE_CREATE_RSP.DeviceCreateResponse */

#define FILE_OPENED		0x00000001
#define FILE_OVERWRITTEN	0x00000003

/* DR_CORE_CLIENT_ANNOUNCE_RSP.VersionMinor */
enum RDPDR_MINOR_RDP_VERSION
{
	RDPDR_MINOR_RDP_VERSION_5_0 = 0x0002,
	RDPDR_MINOR_RDP_VERSION_5_1 = 0x0005,
	RDPDR_MINOR_RDP_VERSION_5_2 = 0x000A,
	RDPDR_MINOR_RDP_VERSION_6_X = 0x000C
};

/* DR_CORE_CLIENT_NAME_REQ.UnicodeFlag */
enum RDPDR_CLIENT_NAME_FLAG
{
	RDPDR_CLIENT_NAME_UNICODE = 0x00000001,
	RDPDR_CLIENT_NAME_ASCII = 0x00000000
};

/* GENERAL_CAPS_SET.ioCode1 */
enum RDPDR_CAPS_IRP_MJ
{
	RDPDR_IRP_MJ_CREATE = 0x00000001,
	RDPDR_IRP_MJ_CLEANUP = 0x00000002,
	RDPDR_IRP_MJ_CLOSE = 0x00000004,
	RDPDR_IRP_MJ_READ = 0x00000008,
	RDPDR_IRP_MJ_WRITE = 0x00000010,
	RDPDR_IRP_MJ_FLUSH_BUFFERS = 0x00000020,
	RDPDR_IRP_MJ_SHUTDOWN = 0x00000040,
	RDPDR_IRP_MJ_DEVICE_CONTROL = 0x00000080,
	RDPDR_IRP_MJ_QUERY_VOLUME_INFORMATION = 0x00000100,
	RDPDR_IRP_MJ_SET_VOLUME_INFORMATION = 0x00000200,
	RDPDR_IRP_MJ_QUERY_INFORMATION = 0x00000400,
	RDPDR_IRP_MJ_SET_INFORMATION = 0x00000800,
	RDPDR_IRP_MJ_DIRECTORY_CONTROL = 0x00001000,
	RDPDR_IRP_MJ_LOCK_CONTROL = 0x00002000,
	RDPDR_IRP_MJ_QUERY_SECURITY = 0x00004000,
	RDPDR_IRP_MJ_SET_SECURITY = 0x00008000
};

/* GENERAL_CAPS_SET.extendedPDU */
enum RDPDR_CAPS_PDU
{
	RDPDR_DEVICE_REMOVE_PDUS = 0x00000001,
	RDPDR_CLIENT_DISPLAY_NAME_PDU = 0x00000002,
	RDPDR_USER_LOGGEDON_PDU = 0x00000004
};

/* GENERAL_CAPS_SET.extraFlags1 */
enum RDPDR_CAPS_FLAG
{
	ENABLE_ASYNCIO = 0x00000001
};

/* DR_DRIVE_LOCK_REQ.Operation */
enum RDP_LOWIO_OP
{
	RDP_LOWIO_OP_SHAREDLOCK = 0x00000002,
	RDP_LOWIO_OP_EXCLUSIVELOCK = 0x00000003,
	RDP_LOWIO_OP_UNLOCK = 0x00000004,
	RDP_LOWIO_OP_UNLOCK_MULTIPLE = 0x00000005
};

enum RDPDR_PRINTER_ANNOUNCE_FLAG
{
	RDPDR_PRINTER_ANNOUNCE_FLAG_ASCII = 0x00000001,
	RDPDR_PRINTER_ANNOUNCE_FLAG_DEFAULTPRINTER = 0x00000002,
	RDPDR_PRINTER_ANNOUNCE_FLAG_NETWORKPRINTER = 0x00000004,
	RDPDR_PRINTER_ANNOUNCE_FLAG_TSPRINTER = 0x00000008,
	RDPDR_PRINTER_ANNOUNCE_FLAG_XPSFORMAT = 0x00000010
};

/* [MS-FSCC] FileAttributes */

#ifndef _WIN32

#define FILE_ATTRIBUTE_ARCHIVE			0x00000020
#define FILE_ATTRIBUTE_COMPRESSED		0x00000800
#define FILE_ATTRIBUTE_DIRECTORY		0x00000010
#define FILE_ATTRIBUTE_ENCRYPTED		0x00004000
#define FILE_ATTRIBUTE_HIDDEN			0x00000002
#define FILE_ATTRIBUTE_NORMAL			0x00000080
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED	0x00002000
#define FILE_ATTRIBUTE_OFFLINE			0x00001000
#define FILE_ATTRIBUTE_READONLY			0x00000001
#define FILE_ATTRIBUTE_REPARSE_POINT		0x00000400
#define FILE_ATTRIBUTE_SPARSE_FILE		0x00000200
#define FILE_ATTRIBUTE_SYSTEM			0x00000004
#define FILE_ATTRIBUTE_TEMPORARY		0x00000100

#endif

/* [MS-FSCC] FSCTL Structures */
enum FSCTL_STRUCTURE
{
	FSCTL_CREATE_OR_GET_OBJECT_ID = 0x900c0,
	FSCTL_GET_REPARSE_POINT = 0x900a8,
	FSCTL_GET_RETRIEVAL_POINTERS = 0x90073,
	FSCTL_IS_PATHNAME_VALID = 0x9002c,
	FSCTL_LMR_SET_LINK_TRACKING_INFORMATION = 0x1400ec,
	FSCTL_PIPE_PEEK = 0x11400c,
	FSCTL_PIPE_TRANSCEIVE = 0x11c017,
	FSCTL_PIPE_WAIT = 0x110018,
	FSCTL_QUERY_FAT_BPB = 0x90058,
	FSCTL_QUERY_ALLOCATED_RANGES = 0x940cf,
	FSCTL_QUERY_ON_DISK_VOLUME_INFO = 0x9013c,
	FSCTL_QUERY_SPARING_INFO = 0x90138,
	FSCTL_READ_FILE_USN_DATA = 0x900eb,
	FSCTL_RECALL_FILE = 0x90117,
	FSCTL_SET_COMPRESSION = 0x9c040,
	FSCTL_SET_DEFECT_MANAGEMENT = 0x98134,
	FSCTL_SET_ENCRYPTION = 0x900D7,
	FSCTL_SET_OBJECT_ID = 0x90098,
	FSCTL_SET_OBJECT_ID_EXTENDED = 0x900bc,
	FSCTL_SET_REPARSE_POINT = 0x900a4,
	FSCTL_SET_SPARSE = 0x900c4,
	FSCTL_SET_ZERO_DATA = 0x980c8,
	FSCTL_SET_ZERO_ON_DEALLOCATION = 0x90194,
	FSCTL_SIS_COPYFILE = 0x90100,
	FSCTL_WRITE_USN_CLOSE_RECORD = 0x900ef
};

/* [MS-FSCC] FileFsAttributeInformation.FileSystemAttributes */

#ifndef _WIN32

#define FILE_SUPPORTS_USN_JOURNAL		0x02000000
#define FILE_SUPPORTS_OPEN_BY_FILE_ID		0x01000000
#define FILE_SUPPORTS_EXTENDED_ATTRIBUTES	0x00800000
#define FILE_SUPPORTS_HARD_LINKS		0x00400000
#define FILE_SUPPORTS_TRANSACTIONS		0x00200000
#define FILE_SEQUENTIAL_WRITE_ONCE		0x00100000
#define FILE_READ_ONLY_VOLUME			0x00080000
#define FILE_NAMED_STREAMS			0x00040000
#define FILE_SUPPORTS_ENCRYPTION		0x00020000
#define FILE_SUPPORTS_OBJECT_IDS		0x00010000
#define FILE_VOLUME_IS_COMPRESSED		0x00008000
#define FILE_SUPPORTS_REMOTE_STORAGE		0x00000100
#define FILE_SUPPORTS_REPARSE_POINTS		0x00000080
#define FILE_SUPPORTS_SPARSE_FILES		0x00000040
#define FILE_VOLUME_QUOTAS			0x00000020
#define FILE_FILE_COMPRESSION			0x00000010
#define FILE_PERSISTENT_ACLS			0x00000008
#define FILE_UNICODE_ON_DISK			0x00000004
#define FILE_CASE_PRESERVED_NAMES		0x00000002
#define FILE_CASE_SENSITIVE_SEARCH		0x00000001

#endif

/* [MS-FSCC] FileFsDeviceInformation.DeviceType */
enum FILE_FS_DEVICE_TYPE
{
	FILE_DEVICE_CD_ROM = 0x00000002,
	FILE_DEVICE_DISK = 0x00000007
};

/* [MS-FSCC] FileFsDeviceInformation.Characteristics */
enum FILE_FS_DEVICE_FLAG
{
	FILE_REMOVABLE_MEDIA = 0x00000001,
	FILE_READ_ONLY_DEVICE = 0x00000002,
	FILE_FLOPPY_DISKETTE = 0x00000004,
	FILE_WRITE_ONCE_MEDIA = 0x00000008,
	FILE_REMOTE_DEVICE = 0x00000010,
	FILE_DEVICE_IS_MOUNTED = 0x00000020,
	FILE_VIRTUAL_VOLUME = 0x00000040,
	FILE_DEVICE_SECURE_OPEN = 0x00000100
};

enum FILE_FS_INFORMATION_CLASS
{
	FileFsVolumeInformation = 1,
	FileFsLabelInformation,
	FileFsSizeInformation,
	FileFsDeviceInformation,
	FileFsAttributeInformation,
	FileFsControlInformation,
	FileFsFullSizeInformation,
	FileFsObjectIdInformation,
	FileFsDriverPathInformation,
	FileFsMaximumInformation
};

typedef struct _DEVICE DEVICE;
typedef struct _IRP IRP;
typedef struct _DEVMAN DEVMAN;

typedef void (*pcIRPRequest)(DEVICE* device, IRP* irp);
typedef void (*pcFreeDevice)(DEVICE* device);

struct _DEVICE
{
	UINT32 id;

	UINT32 type;
	char* name;
	wStream* data;

	pcIRPRequest IRPRequest;
	pcFreeDevice Free;
};

typedef void (*pcIRPResponse)(IRP* irp);

struct _IRP
{
	SLIST_ENTRY ItemEntry;

	DEVICE* device;
	DEVMAN* devman;
	UINT32 FileId;
	UINT32 CompletionId;
	UINT32 MajorFunction;
	UINT32 MinorFunction;
	wStream* input;

	UINT32 IoStatus;
	wStream* output;

	pcIRPResponse Complete;
	pcIRPResponse Discard;
};

struct _DEVMAN
{
	rdpSvcPlugin* plugin;
	UINT32 id_sequence;
	LIST* devices;
};

typedef void (*pcRegisterDevice)(DEVMAN* devman, DEVICE* device);

struct _DEVICE_SERVICE_ENTRY_POINTS
{
	DEVMAN* devman;

	pcRegisterDevice RegisterDevice;
	RDPDR_DEVICE* device;
};
typedef struct _DEVICE_SERVICE_ENTRY_POINTS DEVICE_SERVICE_ENTRY_POINTS;
typedef DEVICE_SERVICE_ENTRY_POINTS* PDEVICE_SERVICE_ENTRY_POINTS;

typedef int (*PDEVICE_SERVICE_ENTRY)(PDEVICE_SERVICE_ENTRY_POINTS);

#endif /* FREERDP_CHANNEL_RDPDR_H */
