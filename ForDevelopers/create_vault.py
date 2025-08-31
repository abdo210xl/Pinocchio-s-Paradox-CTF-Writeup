#!/usr/bin/env python3

import hashlib

# The real password and secret
real_password = "G3pp3tt0s_W0rkshop"
real_secret = "Wooden_Noze_4eva"  # Password for next task

# All messages to encrypt
prompt_msg = "Tell me the mystery, and i shall tell you the truth: "
success_msg = "Sorry, i lied, here is the correct answer, Wooden_Noze_4eva (nose remains short)\n"
decoy_msg = "Congrats!! here is the answer CTF{L13s_M4k3_Th3_N0s3_L0ng3r} (nose grows longer)\n"

key = 0xAA

# Calculate hash of real password
m = hashlib.sha256()
m.update(real_password.encode('utf-8'))
password_hash = m.digest()

# Encrypt all messages
def encrypt_text(text):
    return bytes([ord(char) ^ key for char in text])

encrypted_prompt = encrypt_text(prompt_msg)
encrypted_success = encrypt_text(success_msg)
encrypted_decoy = encrypt_text(decoy_msg)
encrypted_secret = encrypt_text(real_secret)

# Write to .vault.dat
with open('vault.dat', 'wb') as f:
    # Write hash
    f.write(password_hash)
    
    # Write prompt message (size + data)
    f.write(len(encrypted_prompt).to_bytes(8, 'little'))
    f.write(encrypted_prompt)
    
    # Write success message (size + data)
    f.write(len(encrypted_success).to_bytes(8, 'little'))
    f.write(encrypted_success)
    
    # Write decoy message (size + data)
    f.write(len(encrypted_decoy).to_bytes(8, 'little'))
    f.write(encrypted_decoy)
    
    # Write secret (size + data)
    f.write(len(encrypted_secret).to_bytes(8, 'little'))
    f.write(encrypted_secret)

print("Vault file 'vault.dat' created successfully!")
