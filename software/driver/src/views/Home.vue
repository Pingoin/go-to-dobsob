<template>
  <div class="home">
    <img alt="Vue logo" src="../assets/logo.png" />
    <v-select
      v-model="port"
      :items="socket.ports"
      label="Serial Port"
    ></v-select>
    <v-btn elevation="2" @click="socket.setPort(port)">Verbinden</v-btn>
  </div>
</template>

<script lang="ts">
import { Component, Prop, Vue } from "vue-property-decorator";
import Sockethelper from "../plugins/SocketHelper";

@Component
export default class Home extends Vue {
  @Prop() private socket!: Sockethelper;
  port = "";
  constructor() {
    super();
  }
}
</script>
