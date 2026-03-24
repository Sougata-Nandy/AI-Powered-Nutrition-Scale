from flask import Flask, request, jsonify
import os
import requests
from datetime import datetime
from PIL import Image
import io

app = Flask(__name__)
UPLOAD_FOLDER = 'food_uploads'
if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)

# --- CONFIGURATION (Replace with your keys) ---
CLARIFAI_API_KEY = "YOUR_CLARIFAI_KEY"
USDA_API_KEY = "YOUR_USDA_KEY"

def get_dominant_color(image_bytes):
    try:
        img = Image.open(io.BytesIO(image_bytes))
        img = img.resize((1, 1))
        color = img.getpixel((0, 0))
        r, g, b = color[0], color[1], color[2]
        if r > g and r > b: return "Red Fruit/Veg"
        if g > r and g > b: return "Green Vegetable"
        if r > 150 and g > 100 and b < 50: return "Orange/Carrot"
        return "Unknown Food"
    except:
        return "Unknown Food"

@app.route("/upload", methods=["POST"])
def upload_image():
    # 1. Extract Weight from Header
    weight_str = request.headers.get('x-weight', '0')
    real_weight = float(weight_str) if weight_str else 0.0

    # 2. Process Image
    img_data = request.data
    filename = datetime.now().strftime("%Y%m%d_%H%M%S.jpg")
    image_path = os.path.join(UPLOAD_FOLDER, filename)
    with open(image_path, "wb") as f:
        f.write(img_data)

    food = "Unknown"
    cal_100g = 0

    # 3. AI Identification (Clarifai)
    try:
        # (Insert your specific Clarifai API request logic here)
        # If confidence < 0.4 or list empty, trigger fallback:
        if food == "Unknown":
            food = get_dominant_color(img_data)
    except Exception as e:
        print(f"AI Error: {e}")
        food = get_dominant_color(img_data)

    # 4. Nutritional Lookup (USDA)
    try:
        # (Insert your specific USDA API request logic here)
        cal_100g = 52 # Example: Apple default
    except:
        cal_100g = 0

    total_cal = (cal_100g * real_weight) / 100
    print(f"[SUCCESS] {food} | {real_weight}g | {total_cal} kcal")

    return jsonify({
        "status": "success",
        "food": food,
        "weight_g": round(real_weight, 1),
        "total_calories": round(total_cal, 1),
        "calories_per_100g": cal_100g
    })

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, threaded=True)