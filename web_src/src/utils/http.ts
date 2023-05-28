import axios from 'axios'

var token = ''

export const baseUrl = '/api'

export const http = axios.create({
  baseURL: baseUrl,
  headers: {
    token: token,
  },
})

export const setToken = (itoken: string) => {
  token = itoken
  localStorage.setItem('token', token)
  http.defaults.headers['token'] = token
}

export const getToken = (): string => {
  return token
}

const loadToken = () => {
  token =
    localStorage.getItem('token') === null ? '' : localStorage.getItem('token')!
  setToken(token)
}

loadToken()
