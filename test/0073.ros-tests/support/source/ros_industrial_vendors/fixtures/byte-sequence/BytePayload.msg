# Synthetic whole-value codec contract. Not an upstream or robot message.
# Every byte, including embedded NUL and values128..255, is ordinary data.
uint8[] payload
# This trailing scalar detects a partial/prefix-only decoder or encoder.
uint32 sequence
