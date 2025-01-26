import gzip

def compress_html_to_gz_array(html_file_path, output_header_file):
    # Read HTML content
    with open(html_file_path, 'rb') as html_file:
        html_content = html_file.read()

    # Compress HTML content using gzip
    gzipped_content = gzip.compress(html_content)

    # Start preparing the output C array
    array_name = "index_html_gz"
    header_file_contents = f"const uint8_t {array_name}[] PROGMEM = {{\n    "

    # Convert gzipped content to C array format
    line_length = 0
    for byte in gzipped_content:
        # Format each byte as a hex value
        header_file_contents += f"0x{byte:02x}, "
        line_length += 1
        # Limit line length to 16 bytes for readability
        if line_length >= 16:
            header_file_contents += "\n    "
            line_length = 0
    
    # Close the array initialization
    header_file_contents = header_file_contents.rstrip(", \n") + "\n};\n"
    header_file_contents += f"const uint32_t {array_name}_len = {len(gzipped_content)};\n"

    # Write the contents to the output header file
    with open(output_header_file, 'w') as header_file:
        header_file.write("#include <stdint.h>\n")
        header_file.write("#include <avr/pgmspace.h>\n\n")
        header_file.write(header_file_contents)

    print(f"Header file '{output_header_file}' has been generated successfully.")

# Usage
compress_html_to_gz_array('index.html', 'index_html_gz.h')