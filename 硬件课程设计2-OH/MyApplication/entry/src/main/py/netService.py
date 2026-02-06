from flask import Flask, request, jsonify
import modbus_tk.modbus_tcp as modbus_tcp
import modbus_tk.modbus_rtu as modbus_rtu
import modbus_tk.defines as cst
import serial
import logging
import time
from threading import Thread
from threading import Event

logging.basicConfig(
    filename='/usr/local/logs/pythonapp.log',
    level=logging.INFO,
    format='%(asctime)s [%(levelname)s] %(message)s',
)
serial_port = serial.Serial(port='/dev/ttyS2', baudrate=115200, bytesize=8,
parity='N', stopbits=1)
master_tcp = modbus_tcp.TcpMaster(host="192.168.1.100", port=502)
master_rtu = modbus_rtu.RtuMaster(serial_port)
master_rtu.set_timeout(2.0)
master_rtu.set_verbose(True)
command = {
    "forward1": 1,
    "forward2": 2,
    "forward3": 3,
    "forward5": 4,
    "backward1": 5,
    "backward2": 6,
    "backward3": 7,
    "backward5": 8
}
jointAngles = [0, 0, 0, 0, 0, 0]
pos1 = [49, 28, -83, -1, 145, 48]
pos2 = [-51, 32, -108, 4, 158, 40]
isPump = False
redCount = 0
totalCount = 0
# 急停标志：True = 正在急停
emergency_stop = False
# 控制暂停/继续的事件
pause_event = Event()
pause_event.set()
# 中止
auto_task_abort = False

app = Flask(__name__)

@app.before_request
def debug_request():
    print("==== Flask 收到请求 ====")
    print("Path:", request.path)
    print("Method:", request.method)
    print("Headers:", dict(request.headers))
    print("Body:", request.data)
@app.route("/send", methods=["POST"])
def send():
    data = request.json
    mode = data.get("mode")
    jointIndex = data.get("jointIndex")
    direction = data.get("direction")
    degree = data.get("degree")
    logging.info(f'send(): mode: {mode}:{type(mode)}, jointIndex: {jointIndex}:{type(jointIndex)}, 'f'direction: {direction}:{type(direction)}, degree: {degree}:{type(degree)}')
    try:
        if mode == "TCP":
            res = master_tcp.execute(1, cst.WRITE_SINGLE_REGISTER, jointIndex,output_value=command[f"{direction}{degree}"])
        else:
            res = master_rtu.execute(1, cst.WRITE_SINGLE_REGISTER, jointIndex, output_value=command[f"{direction}{degree}"])
        jointAngles[jointIndex] += degree if direction == 'forward' else -degree
        print('jointAngles',jointAngles)
    except Exception as e:
        return jsonify({"error": str(e)})
    return jsonify({"response": res})

@app.route("/reset", methods=["POST"])
def reset():
    data = request.json
    mode = data.get("mode")
    logging.info(f'reset(): mode: {mode}:{type(mode)}')
    try:
        for i in range(5,-1,-1):
            moveJoint(i, 0, mode)
        print('jointAnglesNow ',jointAngles)
    except Exception as e:
        return jsonify({"error": str(e)})
    # return jsonify({"response": res})

@app.route("/pump", methods=["POST"])
def pump():
    data = request.json
    mode = data.get("mode")
    action = data.get("action") # "true" or "false"
    logging.info(f'pump(): mode: {mode}:{type(mode)}, action: {action}:{type(action)}')
    try:
        if (mode == "TCP"):
            res = master_tcp.execute(1, cst.WRITE_SINGLE_REGISTER, 6,output_value=1 if action else 0)
        else:
            res = master_rtu.execute(1, cst.WRITE_SINGLE_REGISTER, 6,output_value=1 if action else 0)
    except Exception as e:
        return jsonify({"error": str(e)})
    return jsonify({"response": res})

@app.route("/conveyor", methods=["POST"])
def conveyor():
    data = request.json
    mode = data.get("mode")
    action = data.get("action")
    logging.info(f'conveyor(): mode: {mode}:{type(mode)}, action: {action}:{type(action)}')
    try:
        if (mode == "TCP"):
            res = master_tcp.execute(1, cst.WRITE_SINGLE_REGISTER, 8,output_value=action)
        else:
            res = master_rtu.execute(1, cst.WRITE_SINGLE_REGISTER, 8,output_value=action)
    except Exception as e:
        return jsonify({"error": str(e)})
    return jsonify({"response": res})

def moveJoint(i, target, mode):
    STEPS = [5, 3, 2, 1]
    while jointAngles[i] != target:
        pause_event.wait()
        delta = target - jointAngles[i]
        for step in STEPS:
            if abs(delta) >= step:
                move = step if delta > 0 else -step
                break
        # 根据 move 决定物理指令
        if move > 0:
            output_value = 4 if step == 5 else step
        else:
            output_value = 8 if step == 5 else step + 4

        if mode == "TCP":
            res = master_tcp.execute(
                1, cst.WRITE_SINGLE_REGISTER, i, output_value=output_value
            )
        else:
            res = master_rtu.execute(
                1, cst.WRITE_SINGLE_REGISTER, i, output_value=output_value
            )

        jointAngles[i] += move
        time.sleep(0.2)
    print('执行关节 ',i, jointAngles)

def readColor(mode):
    if mode == "TCP":
        res = master_tcp.execute(1, cst.READ_HOLDING_REGISTERS, 14, 1)
    else:
        res = master_rtu.execute(1, cst.READ_HOLDING_REGISTERS, 14, 1)
    print('color Res',res)
    return res[0]

def autoControlTask(mode):
    global totalCount,redCount
    try:
        if (mode == "TCP"):
            res = master_tcp.execute(1, cst.WRITE_SINGLE_REGISTER, 8,output_value=1)
        else:
            res = master_rtu.execute(1, cst.WRITE_SINGLE_REGISTER, 8,output_value=1)
        time.sleep(0.2)
        for k in range (10):
            if (mode == "TCP"):
                res = master_tcp.execute(1, cst.WRITE_SINGLE_REGISTER, 7,output_value=1)
            else:
                res = master_rtu.execute(1, cst.WRITE_SINGLE_REGISTER, 7,output_value=1)
            time.sleep(3)
            for i in range(1,6):
                moveJoint(i, pos1[i], mode)
            moveJoint(0, pos1[0], mode)

            global isPump
            if (mode == "TCP"):
                res = master_tcp.execute(1, cst.WRITE_SINGLE_REGISTER, 6,output_value=1)
                isPump = not isPump
            else:
                res = master_rtu.execute(1, cst.WRITE_SINGLE_REGISTER, 6,output_value=1)
                isPump = not isPump
            print('isPump Open',isPump)
            time.sleep(0.2)

            color = readColor(mode)
            if color == 1:
                redCount += 1
            time.sleep(0.2)

            # 2号关节先转到0
            moveJoint(1, 0, mode)
            # 转3-6号关节
            for i in range(2,6):
                moveJoint(i, pos2[i], mode)
            for i in range(0,2):
                moveJoint(i, pos2[i], mode)
            print('moveOver')

            if (mode == "TCP"):
                res = master_tcp.execute(1, cst.WRITE_SINGLE_REGISTER, 6,output_value=2)
                isPump = not isPump
            else:
                res = master_rtu.execute(1, cst.WRITE_SINGLE_REGISTER, 6,output_value=2)
                isPump = not isPump
            print('isPump Close',isPump)
            time.sleep(2)

            totalCount += 1
            res = ledSet(totalCount, mode)
            print('totalCount ',totalCount)
        print('redCount',redCount)
        return res
    except Exception as e:
        logging.error(f"autoControlTask error: {e}")

@app.route("/autoControl", methods=["POST"])
def autoControl():
    data = request.json
    mode = data.get("mode")
    logging.info(f'autoControl(): mode: {mode}:{type(mode)}')
    thread = Thread(target=autoControlTask, args=(mode,))
    thread.start()
    # return jsonify({"response": res})
    return jsonify({"response": "autoControl started"})

@app.route("/getRedCount", methods=["GET"])
def getRedCount():
    global redCount
    return jsonify({
        "redCount": redCount
    })

@app.route("/getTotalCount", methods=["GET"])
def getTotalCount():
    global totalCount
    return jsonify({
        "totalCount": totalCount
    })


def ledSet(index, mode):
    try:
        if index < 10 :
            if (mode == "TCP"):
                res = master_tcp.execute(4, cst.WRITE_SINGLE_REGISTER, 0,output_value = 11)
                time.sleep(0.2)
                res = master_tcp.execute(4, cst.WRITE_SINGLE_REGISTER, 1,output_value= index if index != 0 else 10)
            else:
                res = master_rtu.execute(4, cst.WRITE_SINGLE_REGISTER, 0,output_value = 11)
                time.sleep(0.2)
                res = master_rtu.execute(4, cst.WRITE_SINGLE_REGISTER, 1,output_value= index if index != 0 else 10)
            time.sleep(0.2)
        else:
            tens = index // 10
            ones = index % 10
            if (mode == "TCP"):
                res = master_tcp.execute(4, cst.WRITE_SINGLE_REGISTER, 1,output_value = ones if ones !=0 else 10)
                time.sleep(0.2)
                res = master_tcp.execute(4, cst.WRITE_SINGLE_REGISTER, 0,output_value = tens)
                time.sleep(0.2)
            else:
                res = master_rtu.execute(4, cst.WRITE_SINGLE_REGISTER, 1,output_value = ones  if ones !=0 else 10)
                time.sleep(0.2)
                res = master_rtu.execute(4, cst.WRITE_SINGLE_REGISTER, 0,output_value = tens)
                time.sleep(0.2)
        return res
    except Exception as e:
        return jsonify({"error": str(e)})

@app.route("/setMoveCount", methods=["POST"])
def setMoveCount():
    data = request.json
    mode = data.get("mode")
    selectedIndex = data.get("selectedIndex")
    logging.info(f'setMoveCount(): mode: {mode}:{type(mode)}, selectedIndex: {selectedIndex}:{type(selectedIndex)}')
    res = ledSet(selectedIndex, mode)
    return jsonify({"response": res})

@app.route("/emerStop", methods=["POST"])
def emerStop():
    data = request.json
    mode = data.get("mode")
    global emergency_stop
    emergency_stop = not emergency_stop   # toggle
    if emergency_stop:
        pause_event.clear()   # 立刻暂停
        logging.warning("!!! Emergency STOP triggered !!!")
        return jsonify({"status": "stopped"})
    else:
        pause_event.set()     # 继续执行
        logging.info("Emergency STOP released, continue running")
        return jsonify({"status": "resumed"})

@app.route("/softReset", methods=["POST"])
def softReset():
    data = request.json
    mode = data.get("mode")
    global auto_task_abort

    auto_task_abort = True
    pause_event.set()

    moveJoint(0, pos1[0], mode)
    for i in range(2,6):
        moveJoint(i, pos1[i], mode)
    moveJoint(1, pos1[1], mode)

    if (mode == "TCP"):
        res = master_tcp.execute(1, cst.WRITE_SINGLE_REGISTER, 6,output_value=1)
    else:
        res = master_rtu.execute(1, cst.WRITE_SINGLE_REGISTER, 6,output_value=1)
    time.sleep(0.2)

    if (mode == "TCP"):
        res = master_tcp.execute(1, cst.WRITE_SINGLE_REGISTER, 8,output_value=1)
    else:
        res = master_rtu.execute(1, cst.WRITE_SINGLE_REGISTER, 8,output_value=1)
    time.sleep(0.2)

    for i in range(5,-1,-1):
        moveJoint(i, 0, mode)

    logging.info("softReset completed")
    return jsonify({"status": "soft reset completed"})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)