#ifndef _unzip_H
#define _unzip_H

// UNZIPPING functions -- for unzipping.
// This file is a repackaged form of extracts from the zlib code available
// at www.gzip.org/zlib, by Jean-Loup Gailly and Mark Adler. The original
// copyright notice may be found in unzip.cpp. The repackaging was done
// by Lucian Wischik to simplify and extend its use in Windows/C++. Also
// encryption and unicode filenames have been added.


#ifndef _zip_H
DECLARE_HANDLE(HZIP);
#endif
// An HZIP identifies a zip file that has been opened

typedef DWORD ZRESULT;
// return codes from any of the zip functions. Listed later.

typedef struct
{ int index;                 // index of this file within the zip
  TCHAR name[MAX_PATH];      // filename within the zip
  DWORD attr;                // attributes, as in GetFileAttributes.
  FILETIME atime,ctime,mtime;// access, create, modify filetimes
  long comp_size;            // sizes of item, compressed and uncompressed. These
  long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;


HZIP OpenZip(const TCHAR *fn, const char *password);
HZIP OpenZip(void *z,unsigned int len, const char *password);
HZIP OpenZipHandle(HANDLE h, const char *password);
// OpenZip - opens a zip file and returns a handle with which you can
// subsequently examine its contents. You can open a zip file from:
// from a pipe:             OpenZipHandle(hpipe_read,0);
// from a file (by handle): OpenZipHandle(hfile,0);
// from a file (by name):   OpenZip("c:\\test.zip","password");
// from a memory block:     OpenZip(bufstart, buflen,0);
// If the file is opened through a pipe, then items may only be
// accessed in increasing order, and an item may only be unzipped once,
// although GetZipItem can be called immediately before and after unzipping
// it. If it's opened in any other way, then full random access is possible.
// Note: pipe input is not yet implemented.
// Note: zip passwords are ascii, not unicode.
// Note: for windows-ce, you cannot close the handle until after CloseZip.
// but for real windows, the zip makes its own copy of your handle, so you
// can close yours anytime.

ZRESULT GetZipItem(HZIP hz, int index, ZIPENTRY *ze);
// GetZipItem - call this to get information about an item in the zip.
// If index is -1 and the file wasn't opened through a pipe,
// then it returns information about the whole zipfile
// (and in particular ze.index returns the number of index items).
// Note: the item might be a directory (ze.attr & FILE_ATTRIBUTE_DIRECTORY)
// See below for notes on what happens when you unzip such an item.
// Note: if you are opening the zip through a pipe, then random access
// is not possible and GetZipItem(-1) fails and you can't discover the number
// of items except by calling GetZipItem on each one of them in turn,
// starting at 0, until eventually the call fails. Also, in the event that
// you are opening through a pipe and the zip was itself created into a pipe,
// then then comp_size and sometimes unc_size as well may not be known until
// after the item has been unzipped.

ZRESULT FindZipItem(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
// FindZipItem - finds an item by name. ic means 'insensitive to case'.
// It returns the index of the item, and returns information about it.
// If nothing was found, then index is set to -1 and the function returns
// an error code.

ZRESULT UnzipItem(HZIP hz, int index, const TCHAR *fn);
ZRESULT UnzipItem(HZIP hz, int index, void *z,unsigned int len);
ZRESULT UnzipItemHandle(HZIP hz, int index, HANDLE h);
// UnzipItem - given an index to an item, unzips it. You can unzip to:
// to a pipe:             UnzipItemHandle(hz,i, hpipe_write);
// to a file (by handle): UnzipItemHandle(hz,i, hfile);
// to a file (by name):   UnzipItem(hz,i, ze.name);
// to a memory block:     UnzipItem(hz,i, buf,buflen);
// In the final case, if the buffer isn't large enough to hold it all,
// then the return code indicates that more is yet to come. If it was
// large enough, and you want to know precisely how big, GetZipItem.
// Note: zip files are normally stored with relative pathnames. If you
// unzip with ZIP_FILENAME a relative pathname then the item gets created
// relative to the current directory - it first ensures that all necessary
// subdirectories have been created. Also, the item may itself be a directory.
// If you unzip a directory with ZIP_FILENAME, then the directory gets created.
// If you unzip it to a handle or a memory block, then nothing gets created
// and it emits 0 bytes.
ZRESULT SetUnzipBaseDir(HZIP hz, const TCHAR *dir);
// if unzipping to a filename, and it's a relative filename, then it will be relative to here.
// (defaults to current-directory).


ZRESULT CloseZip(HZIP hz);
// CloseZip - the zip handle must be closed with this function.

unsigned int FormatZipMessage(ZRESULT code, TCHAR *buf,unsigned int len);
// FormatZipMessage - given an error code, formats it as a string.
// It returns the length of the error message. If buf/len points
// to a real buffer, then it also writes as much as possible into there.


// These are the result codes:
#define ZR_OK         0x00000000     // nb. the pseudo-code zr-recent is never returned,
#define ZR_RECENT     0x00000001     // but can be passed to FormatZipMessage.
// The following come from general system stuff (e.g. files not openable)
#define ZR_GENMASK    0x0000FF00
#define ZR_NODUPH     0x00000100     // couldn't duplicate the handle
#define ZR_NOFILE     0x00000200     // couldn't create/open the file
#define ZR_NOALLOC    0x00000300     // failed to allocate some resource
#define ZR_WRITE      0x00000400     // a general error writing to the file
#define ZR_NOTFOUND   0x00000500     // couldn't find that file in the zip
#define ZR_MORE       0x00000600     // there's still more data to be unzipped
#define ZR_CORRUPT    0x00000700     // the zipfile is corrupt or not a zipfile
#define ZR_READ       0x00000800     // a general error reading the file
#define ZR_PASSWORD   0x00001000     // we didn't get the right password to unzip the file
// The following come from mistakes on the part of the caller
#define ZR_CALLERMASK 0x00FF0000
#define ZR_ARGS       0x00010000     // general mistake with the arguments
#define ZR_NOTMMAP    0x00020000     // tried to ZipGetMemory, but that only works on mmap zipfiles, which yours wasn't
#define ZR_MEMSIZE    0x00030000     // the memory size is too small
#define ZR_FAILED     0x00040000     // the thing was already failed when you called this function
#define ZR_ENDED      0x00050000     // the zip creation has already been closed
#define ZR_MISSIZE    0x00060000     // the indicated input file size turned out mistaken
#define ZR_PARTIALUNZ 0x00070000     // the file had already been partially unzipped
#define ZR_ZMODE      0x00080000     // tried to mix creating/opening a zip 
// The following come from bugs within the zip library itself
#define ZR_BUGMASK    0xFF000000
#define ZR_NOTINITED  0x01000000     // initialisation didn't work
#define ZR_SEEK       0x02000000     // trying to seek in an unseekable file
#define ZR_NOCHANGE   0x04000000     // changed its mind on storage, but not allowed
#define ZR_FLATE      0x05000000     // an internal error in the de/inflation code





// e.g.
//
// SetCurrentDirectory("c:\\docs\\stuff");
// HZIP hz = OpenZip("c:\\stuff.zip",0);
// ZIPENTRY ze; GetZipItem(hz,-1,&ze); int numitems=ze.index;
// for (int i=0; i<numitems; i++)
// { GetZipItem(hz,i,&ze);
//   UnzipItem(hz,i,ze.name);
// }
// CloseZip(hz);
//
//
// HRSRC hrsrc = FindResource(hInstance,MAKEINTRESOURCE(1),RT_RCDATA);
// HANDLE hglob = LoadResource(hInstance,hrsrc);
// void *zipbuf=LockResource(hglob);
// unsigned int ziplen=SizeofResource(hInstance,hrsrc);
// HZIP hz = OpenZip(zipbuf, ziplen, 0);
//   - unzip to a membuffer -
// ZIPENTRY ze; int i; FindZipItem(hz,"file.dat",true,&i,&ze);
// char *ibuf = new char[ze.unc_size];
// UnzipItem(hz,i, ibuf, ze.unc_size);
// delete[] ibuf;
//   - unzip to a fixed membuff -
// ZIPENTRY ze; int i; FindZipItem(hz,"file.dat",true,&i,&ze);
// char ibuf[1024]; ZRESULT zr=ZR_MORE; unsigned long totsize=0;
// while (zr==ZR_MORE)
// { zr = UnzipItem(hz,i, ibuf,1024);
//   unsigned long bufsize=1024; if (zr==ZR_OK) bufsize=ze.unc_size-totsize;
//   totsize+=bufsize;
// }
//   - unzip to a pipe -
// HANDLE hwrite; HANDLE hthread=CreateWavReaderThread(&hwrite);
// int i; ZIPENTRY ze; FindZipItem(hz,"sound.wav",true,&i,&ze);
// UnzipItemHandle(hz,i, hwrite);
// CloseHandle(hwrite);
// WaitForSingleObject(hthread,INFINITE);
// CloseHandle(hwrite); CloseHandle(hthread);
//   - finished -
// CloseZip(hz);
// // note: no need to free resources obtained through Find/Load/LockResource
//
//
// SetCurrentDirectory("c:\\docs\\pipedzipstuff");
// HANDLE hread,hwrite; CreatePipe(&hread,&hwrite,0,0);
// CreateZipWriterThread(hwrite);
// HZIP hz = OpenZipHandle(hread,0);
// for (int i=0; ; i++)
// { ZIPENTRY ze;
//   ZRESULT zr=GetZipItem(hz,i,&ze); if (zr!=ZR_OK) break; // no more
//   UnzipItem(hz,i, ze.name);
// }
// CloseZip(hz);
//
//




// Now we indulge in a little skullduggery so that the code works whether
// the user has included just zip or both zip and unzip.
// Idea: if header files for both zip and unzip are present, then presumably
// the cpp files for zip and unzip are both present, so we will call
// one or the other of them based on a dynamic choice. If the header file
// for only one is present, then we will bind to that particular one.
ZRESULT CloseZipU(HZIP hz);
unsigned int FormatZipMessageU(ZRESULT code, TCHAR *buf,unsigned int len);
bool IsZipHandleU(HZIP hz);
#ifdef _zip_H
#undef CloseZip
#define CloseZip(hz) (IsZipHandleU(hz)?CloseZipU(hz):CloseZipZ(hz))
#else
#define CloseZip CloseZipU
#define FormatZipMessage FormatZipMessageU
#endif

// Basic data types
typedef unsigned char  Byte;  // 8 bits
typedef unsigned int   uInt;  // 16 bits or more
typedef unsigned long  uLong; // 32 bits or more
typedef void *voidpf;
typedef void     *voidp;
typedef long z_off_t;


typedef enum {
	IBM_TYPE,     // get type bits (3, including end bit)
		IBM_LENS,     // get lengths for stored
		IBM_STORED,   // processing stored block
		IBM_TABLE,    // get table lengths
		IBM_BTREE,    // get bit lengths tree for a dynamic block
		IBM_DTREE,    // get length, distance trees for a dynamic block
		IBM_CODES,    // processing fixed or dynamic block
		IBM_DRY,      // output remaining window bytes
		IBM_DONE,     // finished last block, done 
		IBM_BAD}      // got a data error--stuck here 
inflate_block_mode;

typedef struct inflate_huft_s inflate_huft;

struct inflate_huft_s {
	union {
		struct {
			Byte Exop;        // number of extra bits or operation
			Byte Bits;        // number of bits in this code or subcode
		} what;
		uInt pad;           // pad structure to a power of 2 (4 bytes for
	} word;               //  16-bit, 8 bytes for 32-bit int's)
	uInt base;            // literal, length base, distance base, or table offset
};

struct inflate_codes_state;
typedef struct inflate_codes_state inflate_codes_statef;

typedef uLong (*check_func) (uLong check, const Byte *buf, uInt len);
voidpf zcalloc (voidpf opaque, unsigned items, unsigned size);
void   zcfree  (voidpf opaque, voidpf ptr);

// inflate blocks semi-private state 
struct inflate_blocks_state {
	
	// mode 
	inflate_block_mode  mode;     // current inflate_block mode 
	
	// mode dependent information 
	union {
		uInt left;          // if STORED, bytes left to copy 
		struct {
			uInt table;               // table lengths (14 bits) 
			uInt index;               // index into blens (or border)
			uInt *blens;             // bit lengths of codes
			uInt bb;                  // bit length tree depth 
			inflate_huft *tb;         // bit length decoding tree 
		} trees;            // if DTREE, decoding info for trees 
		struct {
			inflate_codes_statef 
				*codes;
		} decode;           // if CODES, current state 
	} sub;                // submode
	uInt last;            // true if this block is the last block 
	
	// mode independent information 
	uInt bitk;            // bits in bit buffer 
	uLong bitb;           // bit buffer 
	inflate_huft *hufts;  // single malloc for tree space 
	Byte *window;        // sliding window 
	Byte *end;           // one byte after sliding window 
	Byte *read;          // window read pointer 
	Byte *write;         // window write pointer 
	check_func checkfn;   // check function 
	uLong check;          // check on output 
	
};



typedef struct tm_unz_s
{ unsigned int tm_sec;            // seconds after the minute - [0,59]
unsigned int tm_min;            // minutes after the hour - [0,59]
unsigned int tm_hour;           // hours since midnight - [0,23]
unsigned int tm_mday;           // day of the month - [1,31]
unsigned int tm_mon;            // months since January - [0,11]
unsigned int tm_year;           // years - [1980..2044]
} tm_unz;


// unz_global_info structure contain global data about the ZIPfile
typedef struct unz_global_info_s
{ unsigned long number_entry;         // total number of entries in the central dir on this disk
unsigned long size_comment;         // size of the global comment of the zipfile
} unz_global_info;

// unz_file_info contain information about a file in the zipfile
typedef struct unz_file_info_s
{ unsigned long version;              // version made by                 2 bytes
unsigned long version_needed;       // version needed to extract       2 bytes
unsigned long flag;                 // general purpose bit flag        2 bytes
unsigned long compression_method;   // compression method              2 bytes
unsigned long dosDate;              // last mod file date in Dos fmt   4 bytes
unsigned long crc;                  // crc-32                          4 bytes
unsigned long compressed_size;      // compressed size                 4 bytes
unsigned long uncompressed_size;    // uncompressed size               4 bytes
unsigned long size_filename;        // filename length                 2 bytes
unsigned long size_file_extra;      // extra field length              2 bytes
unsigned long size_file_comment;    // file comment length             2 bytes
unsigned long disk_num_start;       // disk number start               2 bytes
unsigned long internal_fa;          // internal file attributes        2 bytes
unsigned long external_fa;          // external file attributes        4 bytes
tm_unz tmu_date;
} unz_file_info;

typedef enum {        // waiting for "i:"=input, "o:"=output, "x:"=nothing 
	START,    // x: set up for LEN 
		LEN,      // i: get length/literal/eob next 
		LENEXT,   // i: getting length extra (have base) 
		DIST,     // i: get distance next 
		DISTEXT,  // i: getting distance extra 
		COPY,     // o: copying bytes in window, waiting for space
		LIT,      // o: got literal, waiting for output space 
		WASH,     // o: got eob, possibly still output waiting 
		END,      // x: got eob and all data flushed 
		BADCODE}  // x: got error 
inflate_codes_mode;

// inflate codes private state
struct inflate_codes_state {
	
	// mode 
	inflate_codes_mode mode;      // current inflate_codes mode 
	
	// mode dependent information 
	uInt len;
	union {
		struct {
			const inflate_huft *tree;       // pointer into tree 
			uInt need;                // bits needed 
		} code;             // if LEN or DIST, where in tree 
		uInt lit;           // if LIT, literal 
		struct {
			uInt get;                 // bits to get for extra 
			uInt dist;                // distance back to copy from 
		} copy;             // if EXT or COPY, where and how much 
	} sub;                // submode
	
	// mode independent information 
	Byte lbits;           // ltree bits decoded per branch 
	Byte dbits;           // dtree bits decoder per branch 
	const inflate_huft *ltree;          // literal/length/eob tree
	const inflate_huft *dtree;          // distance tree
	
};


typedef enum {
	IM_METHOD,   // waiting for method byte
		IM_FLAG,     // waiting for flag byte
		IM_DICT4,    // four dictionary check bytes to go
		IM_DICT3,    // three dictionary check bytes to go
		IM_DICT2,    // two dictionary check bytes to go
		IM_DICT1,    // one dictionary check byte to go
		IM_DICT0,    // waiting for inflateSetDictionary
		IM_BLOCKS,   // decompressing blocks
		IM_CHECK4,   // four check bytes to go
		IM_CHECK3,   // three check bytes to go
		IM_CHECK2,   // two check bytes to go
		IM_CHECK1,   // one check byte to go
		IM_DONE,     // finished check, done
		IM_BAD}      // got an error--stay here
inflate_mode;

struct inflate_blocks_state;
typedef struct inflate_blocks_state inflate_blocks_statef;


// inflate private state
struct internal_state {
	
	// mode
	inflate_mode  mode;   // current inflate mode
	
	// mode dependent information
	union {
		uInt method;        // if IM_FLAGS, method byte
		struct {
			uLong was;                // computed check value
			uLong need;               // stream check value
		} check;            // if CHECK, check values to compare
		uInt marker;        // if IM_BAD, inflateSync's marker bytes count
	} sub;        // submode
	
	// mode independent information
	int  nowrap;          // flag for no wrapper
	uInt wbits;           // log2(window size)  (8..15, defaults to 15)
	inflate_blocks_statef
		*blocks;            // current inflate_blocks state
	
};


typedef voidpf (*alloc_func) (voidpf opaque, uInt items, uInt size);
typedef void   (*free_func)  (voidpf opaque, voidpf address);



struct internal_state;

typedef struct z_stream_s {
    Byte    *next_in;  // next input byte
    uInt     avail_in;  // number of bytes available at next_in
    uLong    total_in;  // total nb of input bytes read so far
	
    Byte    *next_out; // next output byte should be put there
    uInt     avail_out; // remaining free space at next_out
    uLong    total_out; // total nb of bytes output so far
	
    char     *msg;      // last error message, NULL if no error
    struct internal_state *state; // not visible by applications
	
    alloc_func zalloc;  // used to allocate the internal state
    free_func  zfree;   // used to free the internal state
    voidpf     opaque;  // private data object passed to zalloc and zfree
	
    int     data_type;  // best guess about the data type: ascii or binary
    uLong   adler;      // adler32 value of the uncompressed data
    uLong   reserved;   // reserved for future use
} z_stream;

typedef z_stream *z_streamp;


// unz_file_info_interntal contain internal info about a file in zipfile
typedef struct unz_file_info_internal_s
{
    uLong offset_curfile;// relative offset of local header 4 bytes
} unz_file_info_internal;


typedef struct
{ bool is_handle; // either a handle or memory
bool canseek;
// for handles:
HANDLE h; bool herr; unsigned long initial_offset; bool mustclosehandle;
// for memory:
void *buf; unsigned int len,pos; // if it's a memory block
} LUFILE;



// file_in_zip_read_info_s contain internal information about a file in zipfile,
//  when reading and decompress it
typedef struct
{
	char  *read_buffer;         // internal buffer for compressed data
	z_stream stream;            // zLib stream structure for inflate
	
	uLong pos_in_zipfile;       // position in byte on the zipfile, for fseek
	uLong stream_initialised;   // flag set if stream structure is initialised
	
	uLong offset_local_extrafield;// offset of the local extra field
	uInt  size_local_extrafield;// size of the local extra field
	uLong pos_local_extrafield;   // position in the local extra field in read
	
	uLong crc32;                // crc32 of all data uncompressed
	uLong crc32_wait;           // crc32 we must obtain after decompress all
	uLong rest_read_compressed; // number of byte to be decompressed
	uLong rest_read_uncompressed;//number of byte to be obtained after decomp
	LUFILE* file;                 // io structore of the zipfile
	uLong compression_method;   // compression method (0==store)
	uLong byte_before_the_zipfile;// byte before the zipfile, (>0 for sfx)
	bool encrypted;               // is it encrypted?
	unsigned long keys[3];        // decryption keys, initialized by unzOpenCurrentFile
	int encheadleft;              // the first call(s) to unzReadCurrentFile will read this many encryption-header bytes first
	char crcenctest;              // if encrypted, we'll check the encryption buffer against this
} file_in_zip_read_info_s;


// unz_s contain internal information about the zipfile
typedef struct
{
	LUFILE* file;               // io structore of the zipfile
	unz_global_info gi;         // public global information
	uLong byte_before_the_zipfile;// byte before the zipfile, (>0 for sfx)
	uLong num_file;             // number of the current file in the zipfile
	uLong pos_in_central_dir;   // pos of the current file in the central dir
	uLong current_file_ok;      // flag about the usability of the current file
	uLong central_pos;          // position of the beginning of the central dir
	
	uLong size_central_dir;     // size of the central directory
	uLong offset_central_dir;   // offset of start of central directory with respect to the starting disk number
	
	unz_file_info cur_file_info; // public info about the current file in zip
	unz_file_info_internal cur_file_info_internal; // private info about it
    file_in_zip_read_info_s* pfile_in_zip_read; // structure about the current file if we are decompressing it
} unz_s, *unzFile;


#endif // _unzip_H
