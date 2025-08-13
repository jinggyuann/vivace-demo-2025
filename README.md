# How to be a forensics investigator?
This tutorial will guide you on how to recover a simple file (aka malware which is fake) that was deleted on a pendrive. We will then analyze the malware and find the hidden string in said malware!

# Procedures
1. Run the following commands in the control panel to prepare everything you need for this demonstration
```
# Initialize Ghidra (run in the directory where the ghidraRun file is present)
chmod +x ghidraRun
./ghidraRun

# Initialize Autopsy (the application required to recover files via USB images)
sudo autopsy

# Initialize the formation of a virtual USB stick being plugged into the computer
chmod +x setup.sh
./setup.sh
```

2. Once the codes above have been executed you should see the following
- Autopsy site which can be accessed via http://localhost:9999/autopsy
- Ghidra application
- A RANDOMUSB Folder on your desktop 
<img width="823" height="886" alt="image" src="https://github.com/user-attachments/assets/ecc7b127-6f8e-4a8a-8db5-428abbec7c46" />

3. Now you can access the RANDOMUSB drive and delete the exe that has already been placed into the drive. By deleting this file, you are now unable to recover it, right...? NAWWWW

4. Now run the following command to initate the recovery process
```
chmod +x recovery.sh
./recovery.sh
```

5. Upon running the recovery command the RANDOMUSB drive should dissapear and a **recovery.img** file should appear. This img file is the image file of the USB. This means all the data no matter deleted or not, as long as it was once apart of the USB is stored in said .img file.

6. Access Autopsy and upload the file into the website via the file directory. (will update a clearer example after this cuz im fking lazy and its 2am)

7. Find the deleted file and restore bla bla bla, will update later
