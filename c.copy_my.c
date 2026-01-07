#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BUF_SIZE 4096   /* Size of buffer used for file copying */

/*
 * copy_my
 * Copies the content of a source file to a destination file using
 * Linux system calls only.
 */
int main(int argc, char *argv[])
{
    /* Validate number of command line arguments */
    if (argc != 3)
    {
        const char msg[] = "Usage: copy_my <source_file> <destination_file>\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        return 1;
    }

    /* Open source file for reading */
    int fd_src = open(argv[1], O_RDONLY);
    if (fd_src < 0)
    {
        const char msg[] = "Error: cannot open source file\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        return 1;
    }

    /*
     * Check if destination file already exists.
     * If it exists, ask the user whether to overwrite it.
     */
    if (access(argv[2], F_OK) == 0)
    {
        const char warning[] =
            "Destination file already exists. Overwrite? (y/n)\n";
        char answer;
        char trash;

        while (1)
        {
            /* Prompt user for overwrite confirmation */
            write(STDOUT_FILENO, warning, sizeof(warning) - 1);

            /* Read single character from user */
            if (read(STDIN_FILENO, &answer, 1) <= 0)
            {
                const char err[] = "Error reading input\n";
                write(STDERR_FILENO, err, sizeof(err) - 1);
                close(fd_src);
                return 1;
            }

            /* Clear remaining input until newline */
            while (read(STDIN_FILENO, &trash, 1) > 0 && trash != '\n');

            /* User approved overwrite */
            if (answer == 'y')
                break;

            /* User canceled copy operation */
            if (answer == 'n')
            {
                close(fd_src);
                return 0;
            }
        }
    }

    /*
     * Open destination file for writing.
     * Create it if it does not exist and truncate it if overwrite was approved.
     */
    int fd_dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dst < 0)
    {
        const char msg[] = "Error: cannot open destination file\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        close(fd_src);
        return 1;
    }

    char buffer[BUF_SIZE];
    ssize_t bytes_read;

    /*
     * Main copy loop.
     * Reads chunks of data from source file and writes them to destination file.
     */
    while ((bytes_read = read(fd_src, buffer, BUF_SIZE)) > 0)
    {
        ssize_t bytes_written = 0;

        /*
         * Handle partial writes.
         * write() may write fewer bytes than requested.
         */
        while (bytes_written < bytes_read)
        {
            ssize_t res = write(fd_dst,
                                buffer + bytes_written,
                                bytes_read - bytes_written);
            if (res < 0)
            {
                const char msg[] = "Error writing to destination file\n";
                write(STDERR_FILENO, msg, sizeof(msg) - 1);
                close(fd_src);
                close(fd_dst);
                return 1;
            }
            bytes_written += res;
        }
    }

    /* Handle read error */
    if (bytes_read < 0)
    {
        const char msg[] = "Error reading from source file\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
    }

    /* Close file descriptors */
    close(fd_src);
    close(fd_dst);

    return 0;
}
