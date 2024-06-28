import random

def generate(file, n):
    str_chars = ['A', 'T', 'G', 'C']
    random_str = ''.join(random.choice(str_chars) for _ in range(n))
    file.write(f"{random_str}\n")

def main():
    try:
        with open("hw2_input.txt", "w") as file:
            file.write("5\n$\n")
            for _ in range(5):
                generate(file, 120)
        print("File created successfully.")
    except Exception as e:
        print(f"Error opening file: {e}")

if __name__ == "__main__":
    random.seed()
    main()
