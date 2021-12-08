#include "write_back.c"
#include "curl_init.c"

#define chunk_init \
    struct MemoryStruct chunk;\
    chunk.memory = malloc(1);\
    chunk.size = 0;\
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);\



#define chunk_reset \
    free(chunk.memory);\
    chunk.memory = malloc(1);\
    chunk.size = 0;\
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);\


#define chunk_free \
    free(chunk.memory);\

#include "subject.c"
#include "files.c"
#include "is_dir.c"
#include "add_dir.c"
#include "read_mail.c"
#include "folders.c"
#include "delete_file.c"

#include "delete_folder.c"
#include "send_mail.c"