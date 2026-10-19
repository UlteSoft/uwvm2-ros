"""Verbatim numbered original inventories, independent of historical statuses.

Only requirement names and IDs are extracted. The dated audit's VERIFIED and
PARTIAL fields are deliberately not imported into current acceptance. This
keeps the original50/55/20/23 inventories alongside the additional39 gap cases.
"""
from pathlib import Path
import re

ORIGINAL = "documents/ros/original-task-audit-20261013-evidence/original-task.txt"
SECTIONS = {"security": ("二十八、完整权限攻击测试", "二十九、性能与稳定性测试", 55),
            "examples": ("三十二、必须提供的示例", "三十三、任务实施顺序", 20),
            "acceptance": ("三十四、最终验收标准", "三十五、最终输出要求", 50),
            "outputs": ("三十五、最终输出要求", None, 23)}


def inventories(source):
    """Return all original numbered IDs or refuse ambiguous changed headings."""
    text = (Path(source)/ORIGINAL).read_text()
    result = {}
    for name, (begin, end, count) in SECTIONS.items():
        if text.count(begin) != 1 or end is not None and text.count(end) != 1:
            raise ValueError("original section heading changed")
        body = text.split(begin, 1)[1]
        if end is not None: body = body.split(end, 1)[0]
        if name == "acceptance":
            rows = list(enumerate(re.findall(r"(?m)^\[ \]\s+(.+)$", body), 1))
        else:
            rows = [(int(index), title.strip()) for index, title in re.findall(r"(?m)^([0-9]+)\.\s+(.+)$", body)]
        if [index for index, _ in rows] != list(range(1, count+1)):
            raise ValueError("original numbered inventory changed: "+name)
        result[name] = [{"id": index, "title": title} for index, title in rows]
    chapters = re.findall(r"(?m)^[一二三四五六七八九十]+、(.+)$", text)
    if len(chapters) != 35:
        raise ValueError("original thirty-five chapter inventory changed")
    result["chapters"] = [{"id": index, "title": title} for index, title in enumerate(chapters, 1)]
    phase_body = text.split("三十三、任务实施顺序", 1)[1].split("三十四、最终验收标准", 1)[0]
    phases = re.findall(r"PHASE ([0-9]+)：\n(.*?)(?=\nPHASE |\n每个阶段)", phase_body, flags=re.S)
    if [int(index) for index, _ in phases] != list(range(1, 13)):
        raise ValueError("original twelve phase inventory changed")
    result["phases"] = [{"id": int(index), "title": "PHASE "+index,
        "requirements": [line.strip() for line in body.splitlines() if line.strip()]} for index, body in phases]
    return result
