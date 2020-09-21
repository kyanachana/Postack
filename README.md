# ポスティング対策装置【Postack】

３日振りにポストの中身を見ると…
大量のチラシとたまに重要な書類

チラシは届けてほしくないし、重要な郵便物が来たら知らせてほしい…

そんな願いを叶えたのがこの”Postskack"です！

# DEMO

動画：https://youtu.be/MMPec6hIBYo

自宅のポストにM5Stackを磁石で貼り付けます。

負担は寝ているのですが、ポストの蓋が動いたらジャイロセンサーに反応して、怒り出します。

郵便配達員にしか分からない質問（例：「この家の郵便番号は？」）を投げかけ、答えられれば郵便物、間違えればチラシだと判断します。

郵便物だと判断した場合→自分のLINEに通知する

チラシだと判断した場合→嫌な音を出して撃退する

とても便利なポスティング対策装置です！

# Requirement

* M5Stack Gray
* Arduino 1.8.13

# Usage

http://kyokucho1989.chillout.jp/2019/11/09/ifttt%E3%81%A7%E7%B0%A1%E5%8D%98iot%EF%BC%81m5stack%E3%81%A8line%E3%82%92%E9%80%A3%E6%90%BA%E3%81%99%E3%82%8B%E6%96%B9%E6%B3%95/

このURLを参考にして、M5StackからLINEBotに通知が来るようにします。この時、event_nameはpush_lineにしてください。


```
git clone https://github.com/kyanachana/Postack.git
```
postack_codeを開き、wifiのssidとパスワード、IFTTTのmaker keyを変更してください。

postack_codeをM5Stack Grayに書き込みます。


自宅のポストの蓋に磁石でくっつけたら完成です(≧▽≦)


# Author

Kyanachana