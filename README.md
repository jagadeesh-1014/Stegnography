# 🧩 Image Steganography in C

## 📘 Overview
**Image Steganography** is the process of concealing secret data within an image so that it remains invisible to the human eye.  
This project demonstrates steganography using the **Least Significant Bit (LSB)** technique implemented in **C language**.  
It subtly modifies pixel data in BMP images to embed hidden text or files while keeping the image visually unchanged.

---

## 🎯 Objective
The main objective of this project is to **securely hide confidential information inside an image** and later **retrieve it accurately**.  
It showcases how digital images can serve as a medium for covert communication, protecting sensitive data from unauthorized access.

---

## ⚙️ Working Principle
1. **Encoding Phase:**  
   - Takes a `.bmp` image and a secret text or file as input.  
   - Modifies the least significant bits of image pixels to embed data bits.  

2. **Decoding Phase:**  
   - Reads the modified pixels from the encoded image.  
   - Extracts and reconstructs the hidden data bit by bit.

---

## 💡 Why Steganography?
Unlike encryption, which makes data unreadable but noticeable, **steganography hides the very existence of data**.  
This makes it ideal for **secure communication, digital watermarking, and information protection**.

---

## 🌍 Applications
- 🔒 **Secure Communication:** Hide sensitive messages in plain sight.  
- 🖼️ **Digital Watermarking:** Protect intellectual property rights.  
- ✅ **Data Authentication:** Ensure data integrity and originality.  
- 🛰️ **Defense & Intelligence:** Conceal classified or mission-critical data.  
- 🏥 **Medical & Legal:** Safely store private information within images.

---

## ⚙️ Features
- Encode and decode messages using **BMP** images.  
- Simple **command-line interface** for easy use.  
- Maintains **original image quality** with minimal distortion.  
- Modular code structure (header and source files separated).  
- Works efficiently on standard C compilers (GCC, Turbo C, etc.).

---

## ✅ Advantages
- 🔐 **High Security:** Hidden data remains invisible to unauthorized users.  
- 🧠 **Lightweight:** Minimal computational and memory requirements.  
- 💾 **Data Integrity:** Image quality remains unchanged.  
- 🧩 **Educational Value:** Combines C programming, file handling, and image processing concepts.

---

## 🚀 Future Enhancements
- Add support for **JPEG, PNG, and GIF** formats.  
- Include **password protection or encryption** for extra security.  
- Develop a **Graphical User Interface (GUI)** for ease of use.  
- Integrate **AI-based steganalysis** for robustness and detection resistance.

---

## 🧠 Learning Outcomes
- Understanding of **bitwise operations, file structures, and data encoding**.  
- Insight into **real-world applications of information security**.  
- Experience in **modular programming and debugging** in C.  

---

## 👨‍💻 Author
**Jagadeesh C**  
B.Tech – Electronics and Communication Engineering  
Sree Vidyanikethan Engineering College  

---

⭐ *If you like this project, consider giving it a star on GitHub!*  
