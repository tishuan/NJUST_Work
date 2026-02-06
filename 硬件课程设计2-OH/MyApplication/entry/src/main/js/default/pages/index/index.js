// @ts-ignore
import fetch from '@system.fetch'
export default {
    data: {
        mode: 'TCP',
        pump: false,
        jointSelect: '1号关节',
        joints: ['1号关节', '2号关节', '3号关节', '4号关节', '5号关节', '6号关节'],
        jointIndex: 0,
        conveyor: false,         // 新增：传送带状态
        redCount: 0,
        totalCount: 0,
        timer: null,
        countList: ['0','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20',
            '21','22','23','24','25','26','27','28','29','30','31','32','33','34','35','36','37','38','39','40',
            '41','42','43','44','45','46','47','48','49','50'],
        selectedIndex: 0,
    },
    modeSelect(e) {
        this.mode = e;
        console.log("选择通信方式:", this.mode);
    },
    quit() {
        let app = require('@system.app');
        app.terminate()
    },
    onJointChange(data) {
        this.jointIndex = data.newSelected;
        this.jointSelect = data.newValue;
        console.debug('选中了', this.jointSelect, this.jointIndex)
    },
    rotate(direction, degree) {
        fetch.fetch({
            url: 'http://127.0.0.1:5000/send',
            method: 'POST',
            data: {
                mode: this.mode,
                jointIndex: this.jointIndex,
                direction: direction,
                degree: degree
            },
                success: res => console.log('响应:', res.data),
        });
        console.debug("点击按钮：", this.jointSelect, ' ', direction, " ", degree)
    },
    reset() {
        fetch.fetch({
            url: 'http://127.0.0.1:5000/reset',
            method: 'POST',
            data: { mode: this.mode },
            success: res => console.log('响应:', res.data),
        });
        console.debug('归零');
    },
    bump() {
        fetch.fetch({
            url: 'http://127.0.0.1:5000/pump',
            method: 'POST',
            data: {
                mode: this.mode,
                action: !this.pump,
            },
            success: res => {
                let data = res.data;
                data = JSON.parse(data)
                if(data.error){
                    console.error('error:',data.error)
                }else if(data.response){
                    console.info('res：',data.response)
                    this.pump = !this.pump;
                    console.info('bump off:',this.pump);
                }else{
                    console.warn('unknown message:',data)
                }
            },
            fail: err=>{
                console.error('local connect fail:',err);
            },
        });
        if (this.pump) {
            console.debug('pump off');
        } else {
            console.debug('pump on');
        }
    },
    // 新增：传送带控制
    toggleConveyor() {
        const action = this.conveyor ? 2 : 1; // true->停止, false->启动
        fetch.fetch({
            url: 'http://127.0.0.1:5000/conveyor',
            method: 'POST',
            data: {
                mode: this.mode,
                action: action
            },
            success: res => {
                let data = JSON.parse(res.data);
                if(data.error){
                    console.error('error:', data.error)
                } else {
                    this.conveyor = !this.conveyor;
                    console.info('conveyor状态:', this.conveyor ? '启动':'停止');
                }
            },
            fail: err => console.error('传送带控制失败', err)
        });
    },
    //自动控制10次
    autoControl(){
        fetch.fetch({
            url: 'http://127.0.0.1:5000/autoControl',
            method: 'POST',
            data: { mode: this.mode },
            success: res => {
                console.log('响应:', res.data)
                if (this.timer) return

                this.timer = setInterval(() => {
                    this.getRedCount()
                    this.getTotalCount()
                    // 达到10次搬运则停止定时器
                    if (this.totalCount >= 10) {
                        clearInterval(this.timer)
                        this.timer = null
                        console.debug('搬运次数已达10，停止获取数据')
                    }
                }, 2000)
            },
        });
        console.debug('自动控制，生成箱子');
    },
    getRedCount() {
        fetch.fetch({
            url: 'http://127.0.0.1:5000/getRedCount',
            method: 'GET',
            responseType: 'json',
            success: (res) => {
                this.redCount = res.data.redCount
            },
        })
    },
    getTotalCount() {
        fetch.fetch({
            url: 'http://127.0.0.1:5000/getTotalCount',
            method: 'GET',
            responseType: 'json',
            success: (res) => {
                this.totalCount = res.data.totalCount
            },
        })
    },
    onCountChange(e) {
        this.selectedIndex = e.newSelected
        console.log('设置的搬运次数:', this.selectedIndex)
    },
    setMoveCount(){
        fetch.fetch({
            url: 'http://127.0.0.1:5000/setMoveCount',
            method: 'POST',
            data: {
                mode: this.mode,
                selectedIndex: this.selectedIndex
            },
            success: res => console.log('响应:', res.data),
        });
        console.debug("设置次数", this.selectedIndex)
    },
    Estop(){
        fetch.fetch({
            url: 'http://127.0.0.1:5000/emerStop',
            method: 'POST',
            data: { mode: this.mode },
            success: res => {
                console.log('响应:', res.data)
            },
        });
        console.debug('点击急停');
    },
    softReset(){
        fetch.fetch({
            url: 'http://127.0.0.1:5000/softReset',
            method: 'POST',
            data: { mode: this.mode },
            success: res => {
                console.log('响应:', res.data)
            },
        });
        console.debug('点击软复位');
    }
}
