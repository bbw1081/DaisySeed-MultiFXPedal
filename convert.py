import re
import json
import os
import glob

input_dir = "."  # directory containing your .h files
output_dir = "presets"
os.makedirs(output_dir, exist_ok=True)

header_files = sorted(glob.glob(os.path.join(input_dir, "*.h")))

preset_num = 1
for filename in header_files:
    with open(filename, "r") as f:
        content = f.read()

    # extract the JSON string from between R"({ and })"
    match = re.search(r'R"\((\{.*?\})\)"', content, re.DOTALL)
    if not match:
        print(f"Skipping {filename} - no JSON found")
        continue

    json_str = match.group(1)

    # validate it's proper JSON before writing
    try:
        parsed = json.loads(json_str)
    except json.JSONDecodeError as e:
        print(f"Invalid JSON in {filename}: {e}")
        continue

    output_path = os.path.join(output_dir, f"{preset_num}.json")
    with open(output_path, "w") as f:
        json.dump(parsed, f, indent=4)

    print(f"Wrote {filename} -> {output_path}")
    preset_num += 1

print(f"Done - {preset_num - 1} presets written")
