def clean_file(input_file, output_file):
  """
  Reads a file, removes non-one-or-zero characters from each line, and writes the cleaned data to a new file.

  Args:
      input_file: Path to the input file.
      output_file: Path to the output file.
  """
  with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
    for line in infile:
      clean_line = ''.join(char for char in line if char in '01')
      decimal_value = int(clean_line, 2)
      outfile.write(str(decimal_value)+ '\n')

def remove_spaces(input_file, output_file):
  """
  Reads a file, removes extra spaces from each line (keeping one space between words), and writes the cleaned data to a new file.

  Args:
      input_file: Path to the input file.
      output_file: Path to the output file.
  """
  with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
    for line in infile:
      clean_line = ' '.join(line.split())  # Remove extra spaces, keep one per word
      outfile.write(clean_line + "\n")

# Example usage
input_file = "codes.txt"
output_file = "output.txt"
remove_spaces(input_file, output_file)