import random

try:
    # Generate 300,000 random integers in the specified range and format them as strings prefixed with 'I '
    my_list = ["I " + str(i) for i in random.sample(range(-2147483648, 2147483647), 300000)]

    # Generate 200,000 random choices from the specified set of strings
    my_list2 = [random.choice(["D X", "D M", "D E", "F X", "F M", "F E"]) for _ in range(200000)]

    # Combine both lists and shuffle them
    combined_list = my_list + my_list2
    random.shuffle(combined_list)

    # Write the combined list to a text file
    with open("input.txt", "w") as f:
        f.write("500000\n")
        for item in combined_list:
            f.write("%s\n" % item)
    print("File created successfully.")

except MemoryError:
    print("MemoryError: Not enough memory available to complete this operation.")
except PermissionError:
    print("PermissionError: You do not have permission to write to this directory.")
except FileNotFoundError:
    print("FileNotFoundError: The system cannot find the file specified.")
except Exception as e:
    print(f"An error occurred: {e}")
