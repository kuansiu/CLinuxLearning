#include <my_header.h>

#define CHUNK_SIZE (16 * 1024 * 1024) // 16MB

int main(int argc, char *argv[]) {
    ARGS_CHECK(argc, 3);

    // 1. 打开源文件
    int fd_src = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd_src, -1, "open src");

    // 2. 获取源文件大小
    struct stat st;
    int ret = fstat(fd_src, &st);
    ERROR_CHECK(ret, -1, "fstat src");
    off_t filesize = st.st_size;

    // 3. 打开/创建目标文件，并扩展到相同大小
    int fd_dst = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
    ERROR_CHECK(fd_dst, -1, "open dst");
    ret = ftruncate(fd_dst, filesize);
    ERROR_CHECK(ret, -1, "ftruncate dst");

    // 4. 开始计时
    struct timespec t_start, t_end;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    // 5. 分段映射+复制+日志
    off_t offset = 0;
    size_t chunk_id = 0;
    while (offset < filesize) {
        size_t len = (filesize - offset > CHUNK_SIZE ? CHUNK_SIZE : filesize - offset);

        void *src_map = mmap(NULL, len, PROT_READ, MAP_SHARED, fd_src, offset);
        ERROR_CHECK(src_map, MAP_FAILED, "mmap src");

        void *dst_map = mmap(NULL, len, PROT_WRITE, MAP_SHARED, fd_dst, offset);
        ERROR_CHECK(dst_map, MAP_FAILED, "mmap dst");

        memcpy(dst_map, src_map, len);

        // 可选：立即刷盘
        ret = msync(dst_map, len, MS_SYNC);
        ERROR_CHECK(ret, -1, "msync dst");

        ret = munmap(src_map, len);
        ERROR_CHECK(ret, -1, "munmap src");
        ret = munmap(dst_map, len);
        ERROR_CHECK(ret, -1, "munmap dst");

        offset += len;
        chunk_id++;
        // 打印每段进度
        printf("[Chunk %zu] offset=%lld, len=%zu, progress=%.2f%%\n",
               chunk_id,
               (long long)(offset - len),
               len,
               (double)offset * 100 / filesize);
    }

    // 6. 结束计时
    clock_gettime(CLOCK_MONOTONIC, &t_end);
    double elapsed = (t_end.tv_sec  - t_start.tv_sec)
                   + (t_end.tv_nsec - t_start.tv_nsec) / 1e9;
    printf(">>> 总共复制 %lld 字节，共 %zu 段，耗时 %.3f 秒\n",
           (long long)filesize, chunk_id, elapsed);

    close(fd_src);
    close(fd_dst);
    return 0;
}

