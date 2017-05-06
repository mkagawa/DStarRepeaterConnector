#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/regex.h>
//#include <wx/thread.h>
#include <wx/memory.h> 
#include <wx/log.h> 

#include <pty.h>

const unsigned char DVRPTR_HEADER_LENGTH = 5U;
class CWorkerThread : public wxThread {
  public:
    CWorkerThread(int fd, char* devName);
    virtual ~CWorkerThread();

  protected: 
    // thread execution starts here
    virtual void *Entry() wxOVERRIDE;
    virtual void OnExit() wxOVERRIDE;

  private:
    wxString m_devName;
    int m_fd;
    void ProcessData();

    char m_buffer[1000];
    char m_wbuffer[1000];

    bool m_txEnabled, m_checksum, m_tx, m_txSpace;
    int space;
};

