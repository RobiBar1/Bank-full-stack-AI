/*
Writer:  Robi
Checker: ?
Date:    13.05.2026

comments:
compile in terminal: "gcc src/file_system.c -I include/".
run in terminal: "sudo ./a.out /dev/ram0 /test.txt".
*/

#include <assert.h>
#include <ext2fs/ext2_fs.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file_system.h"

#define BASE_OFFSET 1024
#define BLOCK_OFFSET(block, size) (BASE_OFFSET + (block - 1) * size)
#define INODE_OFFSET(block, size, inode_no, inode_size)                        \
    ((block) * (size) + ((inode_no) - 1) * (inode_size))

void PrintSuperblock(struct ext2_super_block* sb);
void PrintGroupDescriptor(struct ext2_group_desc* gd);
void PrintFileContect();
static void read_inode(int fd, int inode_no,
                       const struct ext2_group_desc* group,
                       struct ext2_inode* inode, unsigned int, unsigned int);
char* strNextWord(char* str, size_t index);
int FindFile(char* buffer, char* target_file, struct ext2_inode* inode);
void PrintFileContent(int fd, struct ext2_inode* inode,
                      unsigned int block_size);

int main(int argc, char* argv[])
{
    struct ext2_super_block superblock = {0};
    struct ext2_group_desc group_desc = {0};
    struct ext2_inode inode = {0};
    unsigned int block_size = 0;
    char* device_path = NULL;
    char* target_file = NULL;
    int result = 0;
    int fd = 0;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <device> <file_path>\n", argv[0]);
        return 1;
    }

    device_path = argv[1];
    target_file = argv[2];

    fd = open(device_path, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening device");
        return 1;
    }

    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &superblock, sizeof(superblock));

    PrintSuperblock(&superblock);
    block_size = 1024 << superblock.s_log_block_size;

    lseek(fd, block_size == 1024 ? 2048 : block_size, SEEK_SET);
    read(fd, &group_desc, sizeof(group_desc));

    PrintGroupDescriptor(&group_desc);
    read_inode(fd, 2, &group_desc, &inode, block_size, superblock.s_inode_size);

    printf("Reading root inode\n"
           "File mode: %hu\n"
           "Owner UID: %hu\n"
           "Size     : %u bytes\n"
           "Blocks   : %u\n",
           inode.i_mode, inode.i_uid, inode.i_size, inode.i_blocks);

    printf("\n--- File Content (%s) ---\n", target_file);
    printf("[Traversal Logic Required]\n");

    for (int i = 0; i < EXT2_N_BLOCKS; i++)
    {
        if (i < EXT2_NDIR_BLOCKS)
        {
            printf("Block %2u : %u\n", i, inode.i_block[i]);
        }
        else if (i == EXT2_IND_BLOCK)
        {
            printf("Single   : %u\n", inode.i_block[i]);
        }
        else if (i == EXT2_DIND_BLOCK)
        {
            printf("Double   : %u\n", inode.i_block[i]);
        }
        else if (i == EXT2_TIND_BLOCK)
        {
            printf("Triple   : %u\n", inode.i_block[i]);
        }
    }

    unsigned char buffer[block_size];

    lseek(fd, block_size * inode.i_block[0], SEEK_SET);
    read(fd, buffer, block_size);

    result = FindFile((char*)buffer, target_file, &inode);

    if (-1 != result)
    {
        struct ext2_inode target_inode = {0};
        read_inode(fd, result, &group_desc, &target_inode, block_size,
                   superblock.s_inode_size);
        PrintFileContent(fd, &target_inode, block_size);
    }
    else
    {
        printf("file not found in root.\n");
    }

    printf("\n--- File Content (%s) ---\n", target_file);

    int target_inode_no = TraversePath(fd, target_file, &group_desc, block_size,
                                       superblock.s_inode_size);

    if (target_inode_no != -1)
    {
        struct ext2_inode target_inode = {0};
        read_inode(fd, target_inode_no, &group_desc, &target_inode, block_size,
                   superblock.s_inode_size);

        /* Ensure we are printing a file, not a directory */
        if (target_inode.i_mode & 0x8000) /* 0x8000 is EXT2_S_IFREG */
        {
            PrintFileContent(fd, &target_inode, block_size);
        }
        else
        {
            printf("Path is a directory, not a file.\n");
        }
    }
    else
    {
        printf("File or path not found.\n");
    }

    close(fd);

    return 0;
}

int TraversePath(int fd, char* path, struct ext2_group_desc* group_desc,
                 unsigned int block_size, unsigned int inode_size)
{
    struct ext2_inode current_inode = {0};
    char path_copy[256] = {0};
    int current_inode_no = 2;
    char buffer[block_size];
    char* token = NULL;

    strncpy(path_copy, path, sizeof(path_copy) - 1);
    token = strtok(path_copy, "/");

    read_inode(fd, current_inode_no, group_desc, &current_inode, block_size,
               inode_size);
    while (token != NULL)
    {
        if (!(current_inode.i_mode & 0x4000))
        {
            return -1;
        }

        lseek(fd, block_size * current_inode.i_block[0], SEEK_SET);
        read(fd, buffer, block_size);

        current_inode_no = FindFile(buffer, token, &current_inode);
        if (-1 == current_inode_no)
        {
            return -1;
        }

        read_inode(fd, current_inode_no, group_desc, &current_inode, block_size,
                   inode_size);
        token = strtok(NULL, "/");
    }

    return current_inode_no;
}

void PrintFileContent(int fd, struct ext2_inode* inode, unsigned int block_size)
{
    char* file_buffer = calloc(1, inode->i_size + 1);
    if (!file_buffer)
    {
        return;
    }

    lseek(fd, block_size * inode->i_block[0], SEEK_SET);
    read(fd, file_buffer, inode->i_size);

    printf("%s\n", file_buffer);
    free(file_buffer);
    file_buffer = NULL;
}

int FindFile(char* buffer, char* target_file, struct ext2_inode* inode)
{
    struct ext2_dir_entry_2* entry = (struct ext2_dir_entry_2*)buffer; ///
    unsigned char* search_name = target_file;
    size_t size = 0;

    if ('/' == search_name[0])
    {
        ++search_name;
    }

    while (size < inode->i_size)
    {
        char file_name[EXT2_NAME_LEN + 1] = {0};
        memcpy(file_name, entry->name, entry->name_len);
        printf("%10u %s\n", entry->inode, file_name);
        if (0 == strcmp(file_name, search_name))
        {
            return entry->inode;
        }

        entry = (struct ext2_dir_entry_2*)((char*)entry + entry->rec_len);
        size += entry->rec_len;
    }

    return -1;
}

char* strNextWord(char* str, size_t index)
{
    char* deli = "/";

    return index != 0 ? strtok(NULL, deli) : strtok(str, deli);
}

static void read_inode(fd, inode_no, group, inode, block_size,
                       inode_size) int fd; /* the floppy disk file descriptor */
int inode_no;                              /* the inode number to read  */
const struct ext2_group_desc*
    group;                /* the block group to which the inode belongs */
struct ext2_inode* inode; /* where to put the inode from disk  */
unsigned int block_size;  /* size of block */
unsigned int inode_size;

{
    lseek(fd,
          INODE_OFFSET(group->bg_inode_table, block_size, inode_no, inode_size),
          SEEK_SET);
    read(fd, inode, inode_size);
} /* read_inode() */

void PrintSuperblock(struct ext2_super_block* sb)
{
    printf("--- Superblock Information ---\n");
    printf("Inodes count      : %u\n", sb->s_inodes_count);
    printf("Blocks count      : %u\n", sb->s_blocks_count);
    printf("First data block  : %u\n", sb->s_first_data_block);
    printf("Block size        : %u bytes\n", 1024 << sb->s_log_block_size);
}

void PrintGroupDescriptor(struct ext2_group_desc* gd)
{
    printf("\n--- Group Descriptor ---\n");
    printf("Block bitmap      : %u\n", gd->bg_block_bitmap);
    printf("Inode bitmap       : %u\n", gd->bg_inode_bitmap);
    printf("Inode table       : %u\n", gd->bg_inode_table);
    printf("Free blocks       : %u\n", gd->bg_free_blocks_count);
    printf("Free inodes       : %u\n", gd->bg_free_inodes_count);
}

void PrintFileContect() {}

// void something(char* virtual_disk_path)
// {
//     struct ext2_super_block super;
//     int fd = open(virtual_disk_path, ...);
//     printf("%d\n", fd);
//     pread(fd, &super, sizeof(super), BASE_OFFSET);
// }