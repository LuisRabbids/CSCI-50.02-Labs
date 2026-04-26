#include <iostream>
#include <string>
#include <ctime>       // for time(), localtime(), strftime()
#include <unistd.h>    // for fork(), execv(), sleep(), getpid()
#include <sys/wait.h>  // for waitpid()
#include <signal.h>    // for kill(), SIGTERM

using namespace std;

int main() {

    // CHILD 1: Launch xclock via execv(), with process name set to "myXclock"
    // Use `whereis xclock` or `which xclock` to verify the path on your system.
    // Common paths: /usr/bin/xclock or /usr/bin/X11/xclock

    pid_t pid1 = fork();

    if (pid1 == -1) {
        // fork() returns -1 on failure
        cerr << "Fork 1 failed." << endl;
        return 1;
    }

    if (pid1 == 0) {
        // We are in Child 1.
        // argv[0] sets the process name visible in `ps aux` as "myXclock".
        char* args[] = { (char*)"myXclock", NULL };

        if (execv("/usr/bin/xclock", args) == -1) {
            // execv only returns if it failed
            cerr << "execv failed. Is xclock installed? Try: sudo apt install x11-apps" << endl;
            return 1;
        }
        // If execv succeeds, this process image is fully replaced; nothing below runs.
    }

    // CHILD 2: Wait for Enter key, print "Terminated.", then exit

    pid_t pid2 = fork();

    if (pid2 == -1) {
        // fork() failed; clean up child 1 before exiting
        cerr << "Fork 2 failed." << endl;
        kill(pid1, SIGTERM);
        return 1;
    }

    if (pid2 == 0) {
        // We are in Child 2.
        string input;
        getline(cin, input); // blocks until Enter is pressed

        cout << "\"Terminated.\"" << endl;
        return 0; // child 2 exits normally
    }

    // PARENT: Print date/time every 3 seconds; exit if any child finishes

    int printCount = 0; // tracks how many times we've printed

    while (true) {

        // Non-blocking check: has any child process exited?
        // waitpid(-1, ...) waits on ANY child; WNOHANG makes it non-blocking.
        int status;
        pid_t finished = waitpid(-1, &status, WNOHANG);

        if (finished > 0) {
            // A child has exited (either xclock was closed or Enter was pressed).
            // Kill the remaining child and exit the parent.
            cout << "\nA child process ended. Shutting down..." << endl;
            kill(pid1, SIGTERM);
            kill(pid2, SIGTERM);
            return 0;
        }

        // Get and format the current date/time as [yyyy-mm-dd] hh:mm:ss
        time_t now = time(nullptr);
        struct tm* timeInfo = localtime(&now);
        char timeBuffer[30];
        strftime(timeBuffer, sizeof(timeBuffer), "[%Y-%m-%d] %H:%M:%S", timeInfo);

        printCount++;
        cout << timeBuffer << endl;

        // Every 3rd printout, show the special message (with literal quotes)
        if (printCount % 3 == 0) {
            cout << "\"This program has gone on for far too long. "
                 << "Close the myXclock window or press Enter on this window to exit.\""
                 << endl;
        }

        // Sleep for 3 seconds — NOT a busy-wait loop.
        // sleep() is a POSIX syscall that suspends the process for N seconds.
        sleep(3);
    }

    return 0;
}