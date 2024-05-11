/**
    This program is offered under a commercial and under the AGPL license.
    For commercial licensing, contact us at kk <kekelanact@gmail.com>.  For AGPL
   licensing, see below.

    AGPL licensing:
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// automatically generated by cbindgen 0.26.0

#include <stdbool.h>
#include <stdint.h>

/**
 * 如果函数返回值是 int，可以通过 SUCCESS 判断是否调用成功
 * 如果返回的是负数，可以调用 get_last_error 获取具体错误信息
 */
#define SUCCESS 0

typedef struct FFIPointerScan FFIPointerScan;

typedef struct FFIModule {
  /**
   * 开始地址
   */
  uintptr_t start;
  /**
   * 结束地址
   */
  uintptr_t end;
  /**
   * 模块路径
   */
  const char *pathname;
} FFIModule;

typedef struct FFIRange {
  /**
   * 向前偏移
   */
  uintptr_t left;
  /**
   * 向后偏移
   */
  uintptr_t right;
} FFIRange;

typedef struct FFIParam {
  /**
   * 三个必选参数
   * 目标地址
   */
  uintptr_t addr;
  /**
   * 扫描深度/最大指针链长度，极大影响指针链计算速度，越小越快
   */
  uintptr_t depth;
  /**
   * 扫描范围，极大影响指针链计算速度，越小越快
   */
  struct FFIRange srange;
  /**
   * 以下为可选参数
   * 强制指定最后一级偏移范围，合理使用会极大加快指针链计算速度，默认 `null`,
   * 与 srange 使用相同的范围
   */
  const struct FFIRange *lrange;
  /**
   * 限制扫描指针链最短长度，会忽略比 `node`
   * 短的指针链，几乎不影响指针链计算速度，默认 `null`, 无限制
   * 如果有值，必须小于或等于 depth
   */
  const uintptr_t *node;
  /**
   * 限制指针链必须以指定的 offset 结尾，几乎不影响指针链计算速度，默认
   * `null`, 无限制
   * 如果有值，必须在 srange 或 lrange 范围内
   */
  const intptr_t *last;
  /**
   * 限制结果中最大指针链条数，到达最大限制函数会提前返回，默认
   * `null`，无限制
   */
  const uintptr_t *max;
  /**
   * 缩短存在循环引用的指针链，例如指针地址 `P1->P2->P3->P1->P5` 会被缩短成
   * `P1->P5`, 略微影响指针链计算速度，默认 `false`，不计算循环引用
   */
  bool cycle;
  /**
   * 下面这些参没有意义，用于控制输出原始指针链的格式，暂时没有实现
   */
  bool raw1;
  bool raw2;
  bool raw3;
} FFIParam;

/**
 * 获取具体错误信息
 */
const char *get_last_error(int code);

/**
 * 初始化
 */
struct FFIPointerScan *ptrscan_init(void);

/**
 * 释放
 */
void ptrscan_free(struct FFIPointerScan *ptr);

/**
 * 获取版本号
 */
const char *ptrscan_version(void);

/**
 * 附加到进程
 */
int ptrscan_attach_process(struct FFIPointerScan *ptr, int32_t pid);

/**
 * 获取可以作为静态基址的模块列表
 */
int ptrscan_list_modules(struct FFIPointerScan *ptr,
                         const struct FFIModule **modules, uintptr_t *size);

/**
 * 在内存中创建指针数据
 * 它是根据传入的基本模块地址范围 `module.start` 以及 `module.end` 创建的。
 * `module.pathname` 是一个文件路径，对于库使用者，你应该根据需要处理这个
 * `module.pathname`, 为了方便库使用者自己解析静态地址，规则是使用者自己订的。
 * 例如只传入文件名而不是整个路径，使用索引处理相同的模块名，
 * 扫描指针链会程序根据 `module.name` 输出静态基址部分的内容。
 * 如果你很懂内存，那也可以根据需要传入特定的地址范围，
 * 例如合并相同模块名的连续区域
 */
int ptrscan_create_pointer_map(struct FFIPointerScan *ptr,
                               const struct FFIModule *modules, uintptr_t size);

/**
 * 在文件中创建指针映射
 * 它是根据传入的基本模块地址范围 `module.start` 以及 `module.end` 创建的。
 * `module.pathname` 是一个文件路径，对于库使用者，你应该根据需要处理这个
 * `module.pathname`, 为了方便库使用者自己解析静态地址，规则是使用者自己订的。
 * 例如只传入文件名而不是整个路径，使用索引处理相同的模块名，
 * 扫描指针链会程序根据 `module.name` 输出静态基址部分的内容。
 * 如果你很懂内存，那也可以根据需要传入特定的地址范围，
 * 例如合并相同模块名的连续区域
 */
int ptrscan_create_pointer_map_file(struct FFIPointerScan *ptr,
                                    const struct FFIModule *modules,
                                    uintptr_t size, const char *pathname);

/**
 * 加载指针映射文件到内存中
 */
int ptrscan_load_pointer_map_file(struct FFIPointerScan *ptr, const char *path);

/**
 * 扫描指针链
 * 它是线程安全的，如果你有多个目标地址参数，可以多线程中同时扫描
 * 关于指针链格式解析，每条以 `$module.name+$offset`
 * 作为静态基址，后续为指针链偏移，以 `.` 分隔，基址 `offset` 和后续偏移都是 10
 * 进制数字
 */
int ptrscan_scan_pointer_chain(struct FFIPointerScan *ptr,
                               struct FFIParam param, const char *pathname);

/**
 * 读取内存
 * 内部维护了进程句柄，使用这个库中的读取内存函数可以直接复用内部进程句柄，
 * 当然你自己重新创建一个进程句柄不用这个函数也没什么问题
 */
int ptrscan_read_memory_exact(struct FFIPointerScan *ptr, uintptr_t addr,
                              uint8_t *data, uintptr_t size);
