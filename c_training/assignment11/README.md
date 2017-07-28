Assignment 11
=========

# 1

Lệnh:
```
# Create local repository
git init R0
```

# 2

Lệnh:
```
# Clone repository Ra from R0
git clone R0/.git Ra
```

# 3

Lệnh:
```
# Create and edit source files
cd Ra
printf "File 1\n" > 1.c
printf "File 2\n" > 2.c
printf "File 3\n" > 3.c
ls -la
git status

# Add all files and commit these changes
git add *.c
git commit -m "Ca0"
git status
```

# 4

Lệnh:
```
# Create/Edit multi source files
printf "File 1 - Line 2\n" >> 1.c
printf "File 2 - Line 2\n" >> 2.c
printf "File 3 - Line 2\n" >> 3.c
git status

# Add some changes (not all)
git add 1.c 2.c

# Commit Ca1
git commit -m "Ca1"
git status
```

# 5

Lệnh:
```
# Commit other changes
git add 3.c
git commit -m "Ca2"
git status
```

# 6

Lệnh:
```
# Modify
printf "File 1 - Line 3\n" >> 1.c
printf "File 2 - Line 3\n" >> 2.c

# Commit 1 file
git add 1.c
git status
git commit -m "Ca3"
git status

# Reset other files (discard un-staged changes)
git reset --hard
```

# 7

Lệnh:
```
# Push master to R0
git push origin master
```

# 8

Lệnh:
```
# Add change to commit Ca3 - Way 1:
printf "File 1 - Line 4\n" >> 1.c
git add 1.c
git commit --amend

# Add change to commit Ca3 - Way 2:
printf "File 1 - Line 4\n" >> 1.c
git add 1.c
# Move HEAD back 1 commit. It means remove commit Ca3
git reset --soft HEAD~1
# Re-commit Ca3
git commit -m "Ca3"
```

Sự khác nhau giữa `git commit --amend` và `git reset --soft` là:

- `git commit --amend` cho phép *sửa* commit gần đây nhất bằng cách thêm các
thay đổi (đã ở trạng thái staged) vào commit đó, đồng thời cũng cho phép thay
đổi commit message.

- `git reset --soft` cho phép *loại bỏ* commit gần nhất khỏi nhánh hiện tại
nhưng vẫn giữ các thay đổi (đã ở trạng thái staged), từ đó ta có thể thêm các
thay đổi mới khác và commit lại.


# 9
Lệnh:
```
# Ta chưa thể đẩy được nhánh master lên origin ngay được vì ta đã sửa commit Ca3
# là commit đã đẩy lên orgin từ trước. Cách xử lí trường hợp này như sau:
# Đầu tiên ta kéo lại nhánh master từ origin
git pull origin master
# Sau đó ta giải quyết xung đột trên tệp 1.c
# Rồi commit mới và đẩy lên origin
git add 1.c
git commit -m "Re-Ca3"
git push origin master
```

# 10
Lệnh:
```
# Clone R0/.git and named Rb
git clone R0/.git Rb
```

# 11
Lệnh:
```
cd Rb

# Xem ID của Ca2 và checkout Ca2
git log
git checkout f9e2d37c68f8e9bf31838784c5b452a8ffaae608

# Tạo nhánh feature1
git checkout -b feature1
```

# 12
Lệnh:
```
printf "File 2 - Line 3" >> 2.c
printf "File 3 - Line 3" >> 3.c

git add 2.c 3.c
git commit -m "Cb1"
```

# 13

Lệnh:
```
printf "File 1 - Line 3\n" >> 1.c

git add 1.c
git commit -m "Cb2"
```

# 14
Lệnh:
```
# Trộn feature1 với master
git merge master
# Giải quyết xung đột ở tệp 1.c xong thì tiếp tục như sau:
git add 1.c
git commit -m "Merge master with feature1"
git checkout master
# Merge feature1 with master
git merge feature1
git push origin master
```

# 15
Lệnh:
```
cp -r R0 R1
```

# 16
Lệnh:
```
git add 1.c
git commit -m "Ca4"
```

# 17
Lệnh:
```
git remote add rmt-r1 ../R1/.git/
```

# 18
Lệnh:
```
# Giải quyết xung đột trước khi đẩy lên rmt-r1
git pull rmt-r1 master
git merge rmt-r1/master

# Đẩy lên rmt-r1
git push rmt-r1 master
```

# 19
Lệnh:
```
# Checkout Ca2
git checkout f9e2d37c68f8e9bf31838784c5b452a8ffaae608

# Create and checkout branch test_rebase
git checkout -b test_rebase
```

# 20
Lệnh:
```
printf "File 1 - Line 3\n" >> 1.c
printf "File 1 - Line 4\n" >> 1.c

git add 1.c
git commit -m "Cat1"

```

# 21
Lệnh:
```
git rebase master
```
