# Assigment 3

## Problem

## Bài 1. Viết 1 hàm tính diện tích hình chữ nhật cho biết chu vi & đường chéo.

Input screen:
`10 4`

Output screen:
`S=xxx`
or `The value inputs are not valid, please re enter new inputs`.

## Bài 2. Viết chương trình bao gồm tính tổng/ tích của 2 ma trận.
Yêu cầu:
- Có hàm nhập ma trận từ bàn phím
- Có hàm in ma trận lên màn hình
- Có hàm kiểm tra xem có nhân được/ cộng được 2 ma trận đã nhập.
- Có hàm tính tổng 2 ma trận
- Có hàm tính tích 2 ma trận

Yêu cầu về output screen:

Input screen:
```
Matrix A Row: 2
Matrix A Col: 2
Matrix B Row: 2
Matrix B Col: 2

Matrix A:
1 2 3 4

Matrix B:
5 6 7 8
```

Output screen:
```
Matrix A, Matrix B can/ cant add together
Matrix A, Matrix B can/cant multi together
Matrix B, Matrix A can/cant multi together

Matrix A:
1 2
3 4

Matrix B:
5 6
7 8

Matrix A + Matrix B: (if cannot, print N/A at here)
6 8
10 12

Matrix A * Matrix B: (if cannot, print N/A at here)
6 8
10 12

Matrix B * Matrix A: (if cannot, print N/A at here)
6 8
10 12
```

## Requirements

Về việc nộp bài:
- Lớp gửi bài cho lớp trưởng theo đúng naming conversion
- Lớp trưởng gửi bài cho anh trước 16h00, 25-Jul-17.

## Analysis

- Nhập 20 số nguyên
- Số trung bình
- Số lượng số nhỏ hơn trung bình
- Sắp xếp chẵn về bên phải, lẻ về bên trái
- Dùng mảng để xử lí kết quả

## Files

- `assigment1.[h\c]`: Utility functions which solve pieces of problem.
- `assignemt1_main.c`: Main entry point. Use utility functions to solve main
problem.