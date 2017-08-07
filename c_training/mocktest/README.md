# MOCK TEST 1

## Day 1 - Tìm hiểu về FAT

### Boot sector là gì?

- Là sector đầu tiên của phân vùng
- Gồm ba phần:
    - Bảng tham số đĩa (BIOS Parameter Block - BPB)
    - Chương trình mồi (Boot strap loader)
    - Chữ kí hệ thống (luôn là 0x55AA)
- Cấu trúc BPB (phần chung):

| STT | Offset | Kích thước (byte) | Giá trị mẫu | Ý nghĩa                                    |
|-----|--------|-------------------|-------------|--------------------------------------------|
| 1   | 0      | 3                 | 0xEB3C90    | Nhảy đến đầu chương trình mồi              |
| 2   | 3      | 8                 | "MSDOS5.0"  | Tên hệ thống file đã format đĩa            |
| 3   | 11     | 2                 | 0x0002      | Kích thước 1 sector, thường là 512         |
| 4   | 13     | 1                 | 0x40        | Số sector cho một cluster (32K-Cluster )   |
| 5   | 14     | 2                 | 0x0100      | Số sector đứng trước FAT/Số sector để dành |
| 6   | 16     | 1                 | 0x02        | Số bảng FAT                                |
| 7   | 17     | 2                 | 0x0002      | Số phần tử của ROOT. FAT32: 0x0000         |
| 8   | 19     | 2                 | 0x0000      |                                            |
| 9   | 21     |                   |             |                                            |
| 10  | 22     |                   |             |                                            |
| 11  | 24     |                   |             |                                            |
| 12  | 16     |                   |             |                                            |
| 13  | 28     |                   |             |                                            |
| 14  | 32     |                   |             |                                            |

- Cấu trúc BPB (tiếp theo, phần của FAT12/FAT16)
- Cấu trúc BPB (tiếp theo, phần của FAT32)

### Cấu trúc của FAT

- File Allocation Table (FAT)
- FAT gồm nhiều phần tử:
    - Kích thước 12 bit, 16 bit, 32 bit
    - Ứng với một khối (cluster) trên vùng dữ liệu
- Cấu trúc
