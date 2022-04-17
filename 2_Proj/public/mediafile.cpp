#include "mediafile.h"

mediaFile::mediaFile()
{
   fileName = filePath = "";
   fileSize = 0;
   recvSize = 0;
   sendSize = 0;
   channelNumber = 0;
}

void mediaFile::reset_status()
{
   fileName = filePath = "";
   fileSize = 0;
   recvSize = 0;
   sendSize = 0;
   channelNumber = 0;
}
