/*
scrbufAnnotation.cpp

Annotation attributes: misc information transfer (true color is the most
important) via mmapped file to host Console2 process
*/
/*
Copyright (c) 2010 Igor Russkih
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "headers.hpp"
#include "scrbufAnnotate.hpp"
#pragma hdrstop

ScrBufAnnotationInfo::ScrBufAnnotationInfo()
{
  wchar_t shareName[255];
  wsprintf(shareName, AnnotationShareName, sizeof(AnnotationInfo), GetConsoleWindow());

//  wprintf(shareName);
//  MessageBox(NULL, shareName, L"far", 0);

  int m_dwSize = 0;
  
  hSharedMem = OpenFileMapping(
					FILE_MAP_ALL_ACCESS, 
					FALSE, 
					shareName);

//  printf("-%d-\n", hSharedMem);

  // TODO: error handling

  char * map = (char*)MapViewOfFile(
  					hSharedMem, 
  					FILE_MAP_ALL_ACCESS, 
  					0, 
  					0, 
  					0);

  header = (AnnotationHeader*)map;
  ptr = NULL;
  if (!header) return;

  ptr = (AnnotationInfo*) (map + header->struct_size);

};

ScrBufAnnotationInfo::~ScrBufAnnotationInfo()
{

  UnmapViewOfFile(ptr);
  CloseHandle(hSharedMem);
 
};
