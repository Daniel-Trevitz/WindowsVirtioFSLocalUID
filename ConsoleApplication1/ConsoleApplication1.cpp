// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <aclapi.h>
#include <fcntl.h>
#include <initguid.h>
#include <lm.h>
#include <setupapi.h>
#include <stdio.h>
#include <sys/stat.h>
#include <wtsapi32.h>
#include <cfgmgr32.h>

int main()
{
   DWORD SessionId;
   PWSTR UserName = NULL;
   LPUSER_INFO_3 UserInfo = NULL;
   DWORD BytesReturned;
   NET_API_STATUS Status;
   BOOL Result;
   
   SessionId = WTSGetActiveConsoleSessionId();
   if (SessionId != 0xFFFFFFFF)
   {
      Result = WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, SessionId,
         WTSUserName, &UserName, &BytesReturned);

      if (Result == TRUE)
      {
         std::wcout << "User: " << (wchar_t*)UserName << std::endl;

         Status = NetUserGetInfo(NULL, UserName, 3, (LPBYTE *)&UserInfo);

         if (Status == NERR_Success)
         {
            // Use an account from local machine's user DB as the file's
            // owner (0x30000 + RID).
            std::cout << "UID: " << std::hex << UserInfo->usri3_user_id + 0x30000 << std::endl;
            std::cout << "GID: " << std::hex << UserInfo->usri3_primary_group_id + 0x30000 << std::endl;
         }
         else
         {
            std::cerr << "Bleh: " << Status << std::endl;
         }

         if (UserInfo != NULL)
         {
            NetApiBufferFree(UserInfo);
         }
         else
         {
            std::cerr << "No Cleanup :(" << std::endl;
         }
      }
      else
      {
         std::cerr << "WTSQuerySessionInformation failed :(" << std::endl;
      }

      if (UserName != NULL)
      {
         WTSFreeMemory(UserName);
      }
      else
      {
         std::cerr << "UserName Not found!" << std::endl;
      }
   }
   else
   {
      std::cerr << "No Session ID!" << std::endl;
   }

    char c;
    std::cin >> c;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
