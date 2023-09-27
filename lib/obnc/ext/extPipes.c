#include ".obnc/extPipes.h"
#include <obnc/OBNC.h>
#include <errno.h>
#include <stdio.h>

#define READ_MODE 0
#define WRITE_MODE 1

typedef struct Stream *Stream;

struct Stream {
	struct extPipes__Stream_ base;
	FILE *file;
	int readWriteMode;
};

struct HeapStream {
	const OBNC_Td *td;
	struct Stream fields;
};

const int extPipes__Stream_id;
const int *const extPipes__Stream_ids[1] = {&extPipes__Stream_id};
const OBNC_Td extPipes__Stream_td = {extPipes__Stream_ids, 1};

static void Open(const char command[], OBNC_INTEGER commandLen, int readWriteMode, extPipes__Stream_ *stream)
{
	FILE *file;
	Stream stream1;
	const char *readWriteModeString;

	*stream = NULL;
	readWriteModeString = (readWriteMode == READ_MODE)? "r": "w";
	file = popen(command, readWriteModeString);
	if (file != NULL) {
		OBNC_NEW(stream1, &extPipes__Stream_td, struct HeapStream, OBNC_REGULAR_ALLOC);
		if (stream1 != NULL) {
			stream1->base.eof_ = 0;
			stream1->file = file;
			stream1->readWriteMode = readWriteMode;
			*stream = (extPipes__Stream_) stream1;
		} else {
			fprintf(stderr, "Pipes.Open failed: out of memory\n");
		}
	} else {
		fprintf(stderr, "Pipes.Open failed: %s\n", strerror(errno));
	}
}


void extPipes__OpenRead_(const char command[], OBNC_INTEGER commandLen, extPipes__Stream_ *stream)
{
	OBNC_C_ASSERT(OBNC_Terminated(command, commandLen));
	Open(command, commandLen, READ_MODE, stream);
}


void extPipes__OpenWrite_(const char command[], OBNC_INTEGER commandLen, extPipes__Stream_ *stream)
{
	OBNC_C_ASSERT(OBNC_Terminated(command, commandLen));
	Open(command, commandLen, WRITE_MODE, stream);
}


void extPipes__Read_(extPipes__Stream_ stream, char *ch)
{
	int ch1;

	OBNC_C_ASSERT(stream != NULL);
	OBNC_C_ASSERT(((Stream) stream)->readWriteMode == READ_MODE);

	ch1 = fgetc(((Stream) stream)->file);
	if (ch1 != EOF) {
		*ch = ch1;
	} else {
		stream->eof_ = 1;
	}
}


void extPipes__Write_(char ch, extPipes__Stream_ stream)
{
	int ch1;

	OBNC_C_ASSERT(stream != NULL);
	OBNC_C_ASSERT(((Stream) stream)->readWriteMode == WRITE_MODE);

	ch1 = fputc(ch, ((Stream) stream)->file);
	if (ch1 == EOF) {
		stream->eof_ = 1;
	}
}


void extPipes__Close_(extPipes__Stream_ stream, OBNC_INTEGER *exitCode)
{
	OBNC_C_ASSERT(stream != NULL);
	OBNC_C_ASSERT(((Stream) stream)->file != NULL);

	*exitCode = pclose(((Stream) stream)->file);
	((Stream) stream)->file = NULL;
}


void extPipes__Init(void)
{
	/*do nothing*/
}
