# 统计词频
# 过滤掉停用词

# 停用词来源：https://github.com/goto456/stopwords

import jieba


def wordcnt_cn(txt_path, out_path=None, stopwords_path=None):
    stopwords = stopwordlist(stopwords_path)
    counts = {}
    with open(txt_path, 'r', encoding='utf-8') as fin:
        lines = fin.readlines()
        for line in lines:
            seg = jieba.cut(line.strip())
            for word in seg:
                # 剔除掉停用词
                if word not in stopwords:
                    if word != ' ':
                        counts[word] = counts.get(word, 0) + 1
    with open(out_path, 'w', encoding='utf-8') as fout:
        for word in counts.keys():
            fout.write(word + ' ' + str(counts[word]) + '\n')


def stopwordlist(stopwords_path):
    with open(stopwords_path, 'r', encoding='utf-8') as fout:
        lines = fout.readlines()
        stopwords = [line.strip() for line in lines]
    return stopwords


if __name__ == '__main__':
    txt_path = "test.txt"
    out_path = "word2cnt.txt"
    stopwords_path = "cn_spotwords.txt"
    wordcnt_cn(txt_path, out_path, stopwords_path)
