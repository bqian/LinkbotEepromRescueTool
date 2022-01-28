<template>
  <div class="home">
    <EepromView name="Serial Id" addr=0x412 size=4 type="char"/>
    <EepromView name="Hardware Version" addr=0x430 size=3 type="byte"/>
  </div>
</template>

<script>
// @ is an alias to /src
import EepromView from '@/components/EepromView.vue'

export default {
  name: 'Home',
  components: {
    EepromView,
  }
}
</script>
