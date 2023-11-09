def create_text_file():
    numbers = '123456789'
    repeat_count = 260000 // len(numbers)
    remaining_chars = 260000 % len(numbers)
    
    content = numbers * repeat_count + numbers[:remaining_chars]
    
    with open('output.txt', 'w') as file:
        file.write(content)

# 이 함수를 호출하여 텍스트 파일을 생성합니다.
create_text_file()
