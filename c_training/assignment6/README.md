# Assignment 6

Bài 1: Viết 1 chương quản lý thông tin của các nhân viên mới vào làm việc cho Fsoft, bao gồm:

- ID (4 bytes)

- Class (1 byte)

- GPA (1byte)

Biết rằng các nhân viên mới này vừa hoàn thành khóa học từ 1 trong 2 lớp fresher A, fresher B.

Yêu cầu:

- Sử dụng link list đơn để quản lý danh sách các nhân viên

- Có các hàm để thêm/bớt 1 nhân viên (2 nhân viên không được trùng ID), sắp xếp lại linked list theo điểm trung bình (GPA) từ cao đến thấp ngay khi thêm/bớt nhân viên.

- Trường hợp 2 nhân viên có điểm trung bình bằng nhau, thì nhân viên nào thuộc lớp A sẽ được tính là cao điểm hơn.

- Trường hợp 2 nhân viên có điểm trung bình bằng nhau và cùng chung lớp A hoặc B thì nhân viên nào được thêm vào trước sẽ được tính là có điểm cao hơn.

- Biết số lượng nhân viên tối đa là 20 bạn. Không được sử dụng các hàm cấp phát bộ nhớ động mà sẽ sử dụng 1 mảng 20 phần tử ( với mỗi phần tử được khởi tạo: ID = 0xFFFFFFFF, Class = 0xFF, Điểm trung bình GPA = 0xFF) để lưu thông tin cho mỗi nhân viên mới. Ngoài các trường ID, Class, GPA thì có thể thêm các trường khác để quản lý nếu cần.

- Trong quá trình thêm nhân viên, nếu có nhân viên xin nghỉ thì sẽ remove nhân viên đó ra khỏi linked list.

## Phân tích

Các thao tác trên danh sách:
- Thêm/bớt: kiểm tra trùng ID, sắp xếp theo GPA từ cao đến thấp
- So sánh:
    - Điểm bằng nhau thì thuộc lớp A coi như cao điểm hơn
    - Điểm bằng nhau và cùng lớp thì thêm trước coi như cao điểm hơn
- Số lượng tối đa: 20
- Không dùng cấp phát động
- Giá trị khởi tạo: ID = ID = 0xFFFFFFFF, Class = 0xFF, GPA = 0xFF

## Ý tưởng
- Thêm/bớt: 