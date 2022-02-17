<template>
  <n-card
    :title="name"
    :segmented="{
      content: true,
      footer: 'soft'
    }"
  >
    <b>Address</b>: {{ addr }}
    <b>size</b>: {{ size }}
    <b>type</b>: {{ type }}
    <b>value</b>: {{ decodedData }}
    <b>bytes</b>: {{ rawData }}
  </n-card>
</template>

<style>
.n-card {
  max-width: 300px;
}
</style>

<script>
export default {
  name: 'EepromView',
  props: {
    name: String,
    addr: Number,
    size: Number,
    type: String,
  },
  data() {
    return {
      bytes: [],
    }
  },
  computed: {
    decodedData() {
      if(this.type === "char") {
        return this.bytes
      } else if(this.type === "float") {
        if(this.bytes.length) {
          var buffer = this.bytes.buffer
          var view = new DataView(buffer.slice(buffer.byteLength-this.size))
          return view.getFloat32(0, true).toFixed(2)
        }
      } else if(this.type === "byte") {
        if(this.size == 1) {
          return this.bytes[0]
        } else {
          var formatted = ''
          this.bytes.forEach((byte, index) => {
            formatted += byte
            if(index != this.bytes.length-1) {
              formatted += '.'
            }
          })
          return formatted
        }
      }

      return ""
    },
    rawData() {
      var formatted = ''
      this.bytes.forEach(byte => {
        formatted += '0x' + byte.toString(16) + ' '
      })

      return formatted
    }
  },
  methods: {
    getEepromData() {
      //this.errorMessage = ''
      let self = this
      var dongle = new window.RobotImpl2('LOCL')
      dongle.connect('ws://localhost:42000', 'LOCL').then(function() {
        dongle.proxy.readEeprom({'address': self.addr, 'size': self.size}, function(result) {
          // check if result contains data
          let data = result.data
          console.log(data)

          if(data.length == self.size) {
            self.bytes = data
          } else {
            //self.errorMessage = 'Error reading Linkbot hardware version'
          }
        })
      })
    },
  },
  compuated: {
    generateTitle() {
      return this.name+ ': ' + this.bytes
    },
  },
  mounted() {
    this.getEepromData()
  },
}
</script>
