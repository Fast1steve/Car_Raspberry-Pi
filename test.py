from wxpy import *
import os
# 初始化机器人，扫码登陆
bot = Bot(None,1)
my_friend = bot.friends().search('AiH')[0]
@bot.register(my_friend)
def reply_my_friend(msg):
    if os.path.exists('./tmp.wav'):
        os.remove('./tmp.wav')
        os.remove('./tmp.mp3')
    msg.get_file('./tmp.mp3')
    os.system('ffmpeg -i ./tmp.mp3 ./tmp.wav')
    os.system('/home/ubuntu/Linux_voice_1151_58f64761/bin/asr_sample /home/ubuntu/tmp.wav')
    return '收到'

embed()
