import serial
import tkinter as tk
from tkinter import messagebox
import customtkinter as ctk
from PIL import Image, ImageTk
import threading
import time
import winsound

# Inisialisasi koneksi serial
ser = serial.Serial(port='COM5', baudrate=9600, timeout=1)  # Sesuaikan port dengan Arduino Anda

# Fungsi untuk memutar efek suara
def play_sound(effect="click"):
    if effect == "click":
        winsound.PlaySound("SystemExclamation", winsound.SND_ASYNC)
    elif effect == "success":
        winsound.PlaySound("SystemAsterisk", winsound.SND_ASYNC)
    elif effect == "error":
        winsound.PlaySound("SystemHand", winsound.SND_ASYNC)

# Fungsi untuk mengirim perintah ke Arduino
def send_command(command):
    try:
        log_activity(f"Sending command: {command}")
        status_label.configure(text="Processing...", text_color="yellow")
        loader.start()
        ser.write(command.encode())
        time.sleep(1)  # Simulasi waktu proses
        loader.stop()
        status_label.configure(text="Command Sent!", text_color="green")
        log_activity(f"Command {command} executed successfully!")
        play_sound("success")
    except Exception as e:
        loader.stop()
        messagebox.showerror("Error", f"Gagal mengirim perintah: {e}")
        status_label.configure(text="Error Sending Command", text_color="red")
        log_activity(f"Error: {e}")
        play_sound("error")

# Fungsi untuk masing-masing gerakan
def Gerakan_1():
    play_sound()
    send_command('A')

def Gerakan_2():
    play_sound()
    send_command('B')

def Gerakan_3():
    play_sound()
    send_command('C')

def Reset():
    play_sound()
    send_command('R')

# Fungsi untuk menampilkan informasi tentang GUI
def show_about():
    play_sound()
    about_window = ctk.CTkToplevel(root)
    about_window.geometry("600x400")
    about_window.title("About RTDS")
    about_window.configure(fg_color="#121212")  # Tema gelap futuristik

    # Menampilkan informasi
    about_label = ctk.CTkLabel(
        about_window,
        text=(
            "RTDS Control Panel\n\n"
            "The purpose of this GUI is to control the RTDS arm, allowing users to generate specific images based on their choices.\n\n"
            "Kinematics of RTDS\n"
            "This GUI is designed to interact with the Kinematics of the RTDS system, which has been developed.\n"
            "The Kinematics component is responsible for defining the motion and positioning of the RTDS arm, ensuring precise and efficient movements to perform desired tasks.\n"
            "The system integrates various computational models and algorithms to achieve high accuracy and reliability in real-time operations.\n\n"
            "GUI Design\n"
            " The graphical user interface (GUI) serves as a user-friendly interaction platform that enables users to input their preferences and commands to the RTDS system.\n"
            "The design focuses on providing an intuitive, efficient, and visually appealing interface for users to control the RTDS arm in generating specific images or executing desired tasks.\n\n"
            "Thank you for using RTDS Control Panel!"
        ),
        font=ctk.CTkFont(size=13),
        text_color="white",
        justify="left",
        wraplength=550
    )
    about_label.pack(pady=20, padx=20)

    # Tombol untuk menutup jendela
    close_button = ctk.CTkButton(
        about_window,
        text="Close",
        command=about_window.destroy,
        fg_color="#FF4500",
        hover_color="#FF6347"
    )
    close_button.pack(pady=20)

# Fungsi untuk mencatat log aktivitas
def log_activity(message):
    console_text.configure(state="normal")
    console_text.insert("end", f"{message}\n")
    console_text.configure(state="disabled")
    console_text.see("end")

# Inisialisasi customtkinter
ctk.set_appearance_mode("Dark")  # Tema futuristik
ctk.set_default_color_theme("dark-blue")

# Membuat root window
root = ctk.CTk()
root.geometry("1000x600")
root.title("RTDS Control Panel")

# Header
header = ctk.CTkLabel(
    root,
    text="RTDS Control Panel",
    font=ctk.CTkFont(size=32, weight="bold"),
    text_color="#00FFFF"
)
header.pack(pady=10)

# Frame utama
main_frame = ctk.CTkFrame(root, fg_color="#1C1C1C", corner_radius=15)
main_frame.pack(pady=20, padx=20, fill="both", expand=True)

# Subframe untuk layout kiri dan kanan
left_frame = ctk.CTkFrame(main_frame, width=400, corner_radius=15, fg_color="#2E2E2E")
left_frame.pack(side="left", padx=10, pady=10, fill="y")

right_frame = ctk.CTkFrame(main_frame, corner_radius=15, fg_color="#121212")
right_frame.pack(side="right", padx=10, pady=10, expand=True, fill="both")

# Tombol kontrol di frame kiri
button_font = ctk.CTkFont(size=16, weight="bold")

ctk.CTkButton(
    left_frame,
    text="Gerakan Setengah Lingkaran",
    command=Gerakan_1,
    font=button_font,
    fg_color="#228B22",
    hover_color="#32CD32"
).pack(pady=10, padx=20, fill="x")

ctk.CTkButton(
    left_frame,
    text="Gerakan Tambah",
    command=Gerakan_2,
    font=button_font,
    fg_color="#4682B4",
    hover_color="#5F9EA0"
).pack(pady=10, padx=20, fill="x")

ctk.CTkButton(
    left_frame,
    text="Gerakan Persegi Panjang",
    command=Gerakan_3,
    font=button_font,
    fg_color="#FFD700",
    hover_color="#FFA500"
).pack(pady=10, padx=20, fill="x")

ctk.CTkButton(
    left_frame,
    text="Reset",
    command=Reset,
    font=button_font,
    fg_color="#DC143C",
    hover_color="#FF6347"
).pack(pady=10, padx=20, fill="x")

# Tombol untuk informasi tentang aplikasi
ctk.CTkButton(
    left_frame,
    text="About",
    command=show_about,
    font=button_font,
    fg_color="#6A5ACD",
    hover_color="#9370DB"
).pack(pady=20, padx=20, fill="x")

# Indikator status dan loader di frame kanan
status_label = ctk.CTkLabel(
    right_frame,
    text="Waiting for Command...",
    font=ctk.CTkFont(size=20, weight="bold"),
    text_color="white"
)
status_label.pack(pady=20)

loader = ctk.CTkProgressBar(right_frame, width=300, height=20, mode="indeterminate")
loader.pack(pady=20)

# Konsol log aktivitas
console_label = ctk.CTkLabel(
    right_frame,
    text="Activity Log",
    font=ctk.CTkFont(size=16, weight="bold"),
    text_color="#00CED1"
)
console_label.pack(pady=10)

console_text = tk.Text(
    right_frame, bg="#1C1C1C", fg="white", font=("Consolas", 12), height=10, state="disabled", wrap="word"
)
console_text.pack(pady=10, padx=10, fill="both", expand=True)

# Tombol untuk keluar dari aplikasi
ctk.CTkButton(
    root,
    text="Quit",
    command=root.quit,
    font=button_font,
    fg_color="#8B0000",
    hover_color="#FF4500"
).pack(pady=20)

root.mainloop()

# Tutup koneksi saat selesai
ser.close()
