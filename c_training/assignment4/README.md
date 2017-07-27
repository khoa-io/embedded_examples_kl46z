# Assignment 4

Đề bài: Viết 1 chương trình lưu danh sách học sinh của một lớp. Có thể thêm và xóa bớt đi.
Các bạn không cần phải viết nhập liệu từ bàn phím. Chỉ cần minh họa bằng cách thêm tên của
học sinh trực tiếp bằng code. Có thể tham khảo các nguồn ở trên mạng nhưng không được copy nguyên.

Ví dụ chương trình:

int main()
{
    // Add student name to list
    insertStudent("Nguyen A");
    insertStudent("Nguyen B");
    insertStudent("Nguyen C");
    insertStudent("Nguyen D");

    // Print all list info
    printAllStudentName();

    // Remove student name from list
    removeStudent("Nguyen A");

    // Print all list info
    printAllStudentName();

    // Add other student name
    insertStudent("Le XYZ");

    // Print all list info
    printAllStudentName();
}

Yêu cầu:
    - Sử dụng linked list để lưu dữ liệu, có tối hiểu các hàm add, remove, find element cho linked list.
    - Đặt tên biến tên hàm tuân thủ theo camel case: chữ cái đầu của từ đầu viết thường, các từ tiếp theo viết hoa (nhìn ví dụ minh họa)
    - Comment đầy đủ cho mỗi line code
    - Gửi assignment theo cấu trúc: <account>_assigment<x>.zip
