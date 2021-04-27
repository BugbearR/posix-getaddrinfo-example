#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void usage() {
    fprintf(stderr, "Usage: posix-getaddrinfo-example [option] host service\n");
    fprintf(stderr, " -4: AF_INET\n");
    fprintf(stderr, " -6: AF_INET6\n");
    fprintf(stderr, " -a: AI_ADDRCONFIG\n");
    fprintf(stderr, " -c: AI_CANONNAME\n");
    fprintf(stderr, " -h: AI_NUMERICHOST\n");
    fprintf(stderr, " -p: AI_PASSIVE\n");
    fprintf(stderr, " -s: AI_NUMERICSERV\n");
    fprintf(stderr, " -t: SOCK_STREAM\n");
    fprintf(stderr, " -u: SOCK_DGRAM\n");
    fprintf(stderr, " -v: AI_V4MAPPED\n");
}

void dump_addrinfo(struct addrinfo *ai)
{
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    int subResult;

    subResult = getnameinfo(ai->ai_addr, ai->ai_addrlen, host, sizeof(host), service, sizeof(service), NI_NUMERICHOST | NI_NUMERICSERV);
    if (subResult != 0)
    {
        fprintf(stderr, "getnameinfo error. %s\n", gai_strerror(subResult));
        return;
    }
    printf("{\n");
    printf("host: \"%s\"\n", host);
    printf(",service: %s\n", service);
    if (ai->ai_canonname)
    {
        printf(",canonname: \"%s\"\n", ai->ai_canonname);
    }
    printf("}\n");
}

int main(int argc, char *argv[])
{
    char opt;
    extern char *optarg;
    extern int optind, opterr;
    struct addrinfo *ai = NULL;
    struct addrinfo *aiwk = NULL;
    struct addrinfo hints = {0};

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    while ((opt = getopt(argc, argv, "46achpstuv")) != -1)
    {
        switch (opt)
        {
        case '4':
            hints.ai_family = AF_INET;
            break;

        case '6':
            hints.ai_family = AF_INET6;
            break;

        case 'a':
            hints.ai_flags |= AI_ADDRCONFIG;
            break;

        case 'c':
            hints.ai_flags |= AI_CANONNAME;
            break;

        case 'h':
            hints.ai_flags |= AI_NUMERICHOST;
            break;

        case 'p':
            hints.ai_flags |= AI_PASSIVE;
            break;

        case 's':
            hints.ai_flags |= AI_NUMERICSERV;
            break;

        case 't':
            hints.ai_socktype = SOCK_STREAM;
            break;

        case 'u':
            hints.ai_socktype = SOCK_DGRAM;
            break;

        case 'v':
            hints.ai_flags |= AI_V4MAPPED;
            break;

        default:
            usage();
            return EXIT_FAILURE;
        }
    }
    argc -= optind;
    argv += optind;

    printf("argc:%d\n", argc);
    if (argc != 2)
    {
        usage();
        return EXIT_FAILURE;
    }

    printf("query: {\n");
    printf("host_name: \"%s\"\n", argv[0]);
    printf("service_name: \"%s\"\n", argv[1]);
    printf("],\n");

    int subResult = getaddrinfo(
        (strcmp("NULL", argv[0]) == 0) ? NULL : argv[0],
        (strcmp("NULL", argv[1]) == 0) ? NULL : argv[1],
        &hints, &ai);
    if (subResult != 0)
    {
        fprintf(stderr, "getaddrinfo error. %s\n", gai_strerror(subResult));
        return EXIT_FAILURE;
    }

    printf("answer: [\n");

    aiwk = ai;
    int count = 0;
    do
    {
        if (count >= 1)
        {
            printf(",");
        }
        dump_addrinfo(aiwk);
        count++;
        aiwk = aiwk->ai_next;
    } while (aiwk);

    printf("]\n");

EXIT_FUNC:
    if (ai) {
        freeaddrinfo(ai);
    }

    return EXIT_SUCCESS;
}
