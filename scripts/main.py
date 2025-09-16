import sys
import os
import urllib.request
import urllib.parse
import json
import csv
import matplotlib.pyplot as plt
from PIL import Image


def main():
    if len(sys.argv) != 2:
        print("scripts/main.py :: Usage: python scripts/main.py <url>")
        return

    raw_url = sys.argv[1]
    print("Fetching:", raw_url)

    try:
        with urllib.request.urlopen(raw_url) as resp:
            body = resp.read().decode("utf-8")
    except Exception as e:
        print("scripts/main.py :: Error fetching URL:", e)
        return

    try:
        parsed = json.loads(body)
    except Exception as e:
        print("scripts/main.py :: Error parsing JSON:", e)
        return

    try:
        u = urllib.parse.urlparse(raw_url)
        q = urllib.parse.parse_qs(u.query)
        function = q.get("function", ["unknown"])[0].lower()
    except Exception as e:
        print("scripts/main.py :: Error parsing URL:", e)
        function = "unknown"

    os.makedirs("data", exist_ok=True)
    os.makedirs("img", exist_ok=True)

    out_file = os.path.join("data", f"{function}.csv")

    dates, values = [], []

    try:
        with open(out_file, "w", newline="", encoding="utf-8") as f:
            writer = csv.writer(f)
            writer.writerow(["Date", "Value"])
            for entry in parsed.get("data", []):
                date = entry.get("date", "")
                value = entry.get("value", "")
                if date and value:
                    writer.writerow([date, value])
                    dates.append(date)
                    try:
                        values.append(float(value))
                    except ValueError:
                        values.append(None)  # Skip invalid numbers
    except Exception as e:
        print("scripts/main.py :: Error writing CSV:", e)
        return

    print("scripts/main.py :: CSV saved to", out_file)

    if dates and values:
        try:
            plt.figure(figsize=(10, 5))
            plt.plot(dates, values, color="blue", label=function)
            plt.xlabel("Date")
            plt.ylabel("Value")
            plt.title(f"Time Series Data: {function.upper()}")
            plt.xticks(rotation=45)
            plt.legend()
            plt.tight_layout()

            png_path = os.path.join("img", f"{function}.png")
            plt.savefig(png_path, dpi=150)
            plt.close()
            print("scripts/main.py :: PNG chart saved to", png_path)
            
            img = Image.open(png_path)
            img = img.resize((350, 200), Image.LANCZOS)
            img.convert("RGB").save(bmp_path, "BMP")
            print("scripts/main.py :: BMP image saved to", bmp_path)            

            try:
                os.remove(out_file)
                print("scripts/main.py :: Deleted temporary CSV:", out_file)
            except Exception as e:
                print("scripts/main.py :: Warning - could not delete CSV:", e)

            try:
                os.remove(png_path)
                print("scripts/main.py :: Deleted temporary PNG:", png_path)
            except Exception as e:
                print("scripts/main.py :: Warning - could not delete PNG:", e)

        except Exception as e:
            print("scripts/main.py :: Error creating chart:", e)


if __name__ == "__main__":
    main()
