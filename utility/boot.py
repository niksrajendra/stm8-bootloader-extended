import serial, os, math, argparse
from time import sleep

BLOCK_SIZE = 64 # 128 for parts with >8k flash

REQ_ENTER = (0xde, 0xad, 0xbe, 0xef)
ACK  = (0xaa, 0xbb)
NACK = (0xde, 0xad)
FILE = None

def crc8_update(data, crc):
    crc ^= data
    for i in range(0, 8):
        if crc & 0x80 != 0:
            crc = (crc << 1) ^ 0x07
        else:
            crc <<= 1
    return crc & 0xFF

def get_crc():
    crc = 0x0
    data = open(FILE, 'rb')
    with data as f:
        chunk = bytearray(f.read(BLOCK_SIZE))
        while chunk:
            chunk.extend([0xFF] * (BLOCK_SIZE - len(chunk)))
            for i in chunk:
                crc = crc8_update(i, crc)
            chunk = bytearray(f.read(BLOCK_SIZE))
    return crc

def bootloader_enter(ser):
    # toggle reset via DTR
    ser.setDTR(True)
    sleep(0.1)
    ser.setDTR(False)
    # send payload
    req = bytearray(REQ_ENTER)
    chunks = os.path.getsize(FILE)
    chunks = int(math.ceil(float(chunks) / BLOCK_SIZE))
    print('Need to send %s chunks' % chunks)
    crc = get_crc()
    req.extend([chunks, crc, crc])
    ser.write(req)
    #ser.flushOutput()
    return ser

def bootloader_exec(port, baud=115200):
    ser = serial.Serial(port, baud, timeout=2.0, write_timeout=0.5, inter_byte_timeout=2, bytesize=8, parity='N', 
    stopbits=1)
    sleep(1)
    bootloader_enter(ser)
    ser.flushInput()
    rxbuff = ser.read(4)
    print(rxbuff)
    data = open(FILE, 'rb')
    total = 0
    with data as f:
        chunk = bytearray(f.read(BLOCK_SIZE))
        while chunk:
            rx = ser.read(2)
            print(rx)
            if len(rx) != 2:
                print('Timeout')
                return
            total += len(chunk)
            print(total)
            chunk.extend([0xFF] * (BLOCK_SIZE - len(chunk)))
            ser.write(chunk)
            #ser.flushOutput()
            #rxbuff = ser.read(BLOCK_SIZE)
            #print(rxbuff)
            chunk = bytearray(f.read(BLOCK_SIZE))
        #bytesToRead = ser.inWaiting()
        ack = ser.read(2)
        print(ack)
        if ack == bytearray(ACK):
            print('Done')
        elif ack == bytearray(NACK):
            print('CRC mismatch')
        else:
            print('Invalid response')
    ser.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='stm8-bootloader update utility')
    parser.add_argument('--port', '-p', default='/dev/ttyUSB0')
    parser.add_argument('--baud', '-b', default=115200)
    parser.add_argument('file', help='firmware in binary format')
    args = parser.parse_args()
    FILE = args.file
    bootloader_exec(args.port, args.baud)
