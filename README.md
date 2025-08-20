# Pinocchio's Paradox - CTF Challenge

![Reverse Engineering](https://img.shields.io/badge/Category-Reverse_Engineering-brightgreen) 
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-green)
![Tools](https://img.shields.io/badge/Tools-Ghidra%2C%20steghide%2C%20strings-blue)

## 📖 Challenge Description

A deceptive binary that lies about its output, leading to a hidden flag embedded in an image using steganography.

**Challenge Files:**
- `pinocchio_vault` - The main challenge binary
- `pinocchio.jpg` - Image containing hidden flag  
- `.vault.dat` - Encrypted data file

## 🎯 Challenge Overview

This is a two-stage challenge:
1. **Reverse Engineering** - Analyze and patch the binary to extract a password
2. **Steganography** - Use the password to reveal a hidden flag in the image

This write-up is divied into two parts:
1. First part: overview of how the challenge was made to help ctf developers
2. Second part: the actual step by step guide to solve the challenge

I will provide initial files for both phases in separate folders in this repo

## 🛠️ For Challenge Developers

### File Structure

├── pinocchio_vault.c # Source code

├── create_vault.py # Vault creation script

├── pinocchio.jpg # Target image

├── flag.txt # Final flag to hide

├── pinocchio_vault # Compiled binary

└── .vault.dat # Generated data file


### 1. Develop the Binary
**pinocchio_vault.c** features:
- SHA-256 password hashing
- XOR string encryption (key: `0xAA`)
- Backwards logic (wrong passwords show decoy flags)(CTF{L13s_M4k3_Th3_N0s3_L0ng3r})
- Requires Ghidra patching

### 2. Prepare Steganography
- **flag.txt** contains: CTF{P1n0k10_4lw4ys_T3lls_Th3_Truth}
- **Embed the flag:**
```bash
steghide embed -cf pinocchio.jpg -ef flag.txt -p "Wooden_Noze_4eva"
```

### 3. Create Vault System
create_vault.py generates .vault.dat with:

- **Hash of password**: G3pp3tt0s_W0rkshop

- **Encrypted messages** using XOR 0xAA to prevent strings command from revealing printf outputs

- **Encrypted secret**: Wooden_Noze_4eva

### 4. Run the Script and Compile the Binary
```bash
python3 create_vault.py
gcc -o pinocchio_vault pinocchio_vault.c -lcrypto -no-pie
```

### 5. Test the Challenge
Entering false passwords outputs a decoy flag, and since obtaining the original password is extremely hard so only ghidra patching is the valid choice
```bash
# Test wrong password
./pinocchio_vault
> Enter password: wrong
> Congrats!! here is the answer CTF{L13s_M4k3_Th3_N0s3_L0ng3r} #decoy flag

# After reversing this program's logic in ghidra
./pinocchio_vault
> Enter password: wrong
> Sorry, i lied, here is the correct answer, Wooden_Noze_4eva

# Test steghide extraction  
steghide extract -sf pinocchio.jpg -p "Wooden_Noze_4eva"
cat flag.txt
> CTF{P1n0k10_4lw4ys_T3lls_Th3_Truth}
```



## 🔍 For Participants: Solution Guide
## Stage 1: Reverse Engineering
### Step 1: Initial Analysis
```bash
strings pinocchio_vault
```
No meaningful strings found due to XOR encryption

### Step 2: Ghidra Decompilation

Here is the output after importing pinocchio.exe


<img width="399" height="552" alt="image" src="https://github.com/user-attachments/assets/7a1e0bac-315a-4ef4-9856-7b37185d83c6" /> 
<img width="343" height="304" alt="image" src="https://github.com/user-attachments/assets/daabf301-8910-48b0-9ecf-8d1651320bd7" />


Decompiled main function looks awful, since there is alot of file reading, but since this is an easy challenge , and it asks for a password, we should try to look for comparison conditions, in our case here second image(if (iVar == 0))

### Step 3: Binary Patching
Find and patch the conditional jump by clicking on the line of decompiled code that you want to change:

<img width="604" height="138" alt="image" src="https://github.com/user-attachments/assets/fc520c9a-d124-462d-8e09-e2d76cd7fee0" />

at this point, you should have the minimum knowledge about assembly instructions

here jnz and jz are used for comparing equality and inequality between two operands

so all what we have to do is change jnz to jz

at this point, ghidra shows its powers, as doing this is pretty simple just follow the images bellow

1. right click on the instruction and click patch instruction

<img width="667" height="304" alt="image" src="https://github.com/user-attachments/assets/90e4700f-57ed-4a18-8f26-4708d169d70a" />

2. a text editor is open and you can edit the instruction

<img width="621" height="213" alt="image" src="https://github.com/user-attachments/assets/0bf03519-1fad-4f19-9bbd-4b9d6909da9f" />

Voila! Just export the new exe program and that's it

### Step 4: Extract Password
No run patched binary:

```bash
./pinocchio_vault_patched
> Tell me the mystery: any_input
> Sorry, i lied, here is the correct answer, Wooden_Noze_4eva
Password extracted: Wooden_Noze_4eva
```

## Stage 2: Steganography
### Step 5: Extract Hidden Flag
```bash
steghide extract -sf pinocchio.jpg -p "Wooden_Noze_4eva"
```
### Step 6: Retrieve Flag
```bash
cat flag.txt
```
## 🏆 Final Flag
### CTF{P1n0k10_4lw4ys_T3lls_Th3_Truth}



